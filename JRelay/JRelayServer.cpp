//
// Made by Jordan Hoosman
//

#ifndef __JRELAYSERVER__
    #include "JRelayServer.h"
#endif

/***********************************************************************************\
|*   Server - Public
\***********************************************************************************/
JRelay::Server::Server(int DesiredPort, int MaxAllowedConnections, ServerDataHandle DataHandle) {
    Server(DesiredPort, MaxAllowedConnections, DataHandle, JRelay::Encryption::Unencrypted);
}
JRelay::Server::Server(int DesiredPort, int MaxAllowedConnections, ServerDataHandle DataHandle, Encryption Encrypt) {
    #ifdef _WIN32 
	if (WSAStartup(DLLVersion, &WSAData) != 0) return false; 
	#endif

    srand(time(NULL));
    
    Port = DesiredPort;
    MaxConnections = MaxAllowedConnections;
    DataHandler = DataHandle;
    EType = Encrypt;

    DATAINFOEMPTY.sender = 0;
    DATAINFOEMPTY.variable = NONNULL_DATA;
    DATAINFOEMPTY.value = NONNULL_DATA;

    DATAINFONULL.sender = 0;
    DATAINFONULL.variable = UNKNOWN_DATA;
    DATAINFONULL.value = UNKNOWN_DATA;

    NULLPCACHE.name = UNKNOWN_DATA;
    NULLPCACHE.value = NULL;
}
void JRelay::Server::setClientInitFunction(ClientConnectedHandle InitFunction) {
    ClientConnectedHandler = InitFunction;
}
void JRelay::Server::setClientDiscFunction(ClientDisconnectedHandle DiscFunction) {
    ClientDisconnectHandler = DiscFunction;
}
void JRelay::Server::enableDisconnectCheck(int PeriodMilliseconds) {
    DisconnectCheckPeriod = PeriodMilliseconds;
    CheckForDisconnect = true;
}
bool JRelay::Server::start() {
    if (!initNetworkSocket(NetworkSocket)) return false;

	int opt = 1, reuseport;
	#if defined(_WIN32)
		reuseport = SO_REUSEADDR;
		if (::setsockopt(NetworkSocket, SOL_SOCKET, reuseport, (char*)&opt, sizeof(opt)) < 0) {
            ERRORLOG.push_back("Network Error: Failed to setsockopt\n");
            return false;
        }
	#else
		reuseport = SO_REUSEADDR | SO_REUSEPORT;
		if (::setsockopt(NetworkSocket, SOL_SOCKET, reuseport, &opt, sizeof(opt)) < 0) {
            ERRORLOG.push_back("Network Error: Failed to setsockopt\n");
            return false;
        }
    #endif
    
    SocketADDR.sin_addr.s_addr = INADDR_ANY;
    SocketADDR.sin_port = htons(Port);
    SocketADDR.sin_family = AF_INET;
    if (::bind(NetworkSocket, (SOCKADDR*)&SocketADDR, SALen) < 0){
        ERRORLOG.push_back("Network Error: Failed to bind\n");
        return false;
    }

    if (::listen(NetworkSocket, SOMAXCONN) < 0) {
        ERRORLOG.push_back("Network Error: Failed to listen\n");
        return false;
    }
    
    ServerRunning = true;
    CanAcceptConnections = true;
    
    if (DataIsPolled) {
        if (!setNonBlocking(NetworkSocket)) {
            ERRORLOG.push_back("Failed to set blocking");
            return false;
        }

        FDTimer.tv_sec = 0;
        FDTimer.tv_usec = FDDelayUS;
    } else {
        std::thread listenthread = std::thread(&Server::listenForConnections, this);
        listenthread.detach();
    }

    return true;
}
void JRelay::Server::turnoff() {
    ServerRunning = false;
    CanAcceptConnections = false;
}
void JRelay::Server::turnon() {
    ServerRunning = true;
}
void JRelay::Server::waitForTurnOff(int Timeout) {
    uint64_t timeend, timenow = std::chrono::system_clock::now().time_since_epoch() / std::chrono::milliseconds(1);

    do {
        std::this_thread::sleep_for(std::chrono::milliseconds(5));
        timeend = std::chrono::system_clock::now().time_since_epoch() / std::chrono::milliseconds(1);
    } while (Connections.size() > 0 && timeend - timenow < Timeout);
}
void JRelay::Server::close() {
    ServerClosed = true;
    ServerRunning = false;
    CanAcceptConnections = false;

    for (const auto& c : Connections) closesoc(c.second.socket);
    Connections.clear();
    closesoc(NetworkSocket);
}
void JRelay::Server::sendDataToClient(int ClientID, std::string Data) {
    if (SendDataOnThread) { std::thread th = std::thread(&Server::sendDataOne, this, ClientID, Data); th.detach(); } 
	else sendDataOne(ClientID, Data);
}
void JRelay::Server::sendDataToAllClients(std::string Data) {
    if (SendDataOnThread) { std::thread th = std::thread(&Server::sendDataAll, this, Data); th.detach(); } 
	else sendDataAll(Data);
}
bool JRelay::Server::hasMaxConnections() {
    return Connections.size() == MaxConnections;
}
int JRelay::Server::numConnected() {
    return Connections.size();
}
void JRelay::Server::removeClient(int ClientID) {
    handleClientDisconnect(ClientID, false);
}

bool JRelay::Server::isOpen() {
    return CanAcceptConnections && Connections.size() < MaxConnections;
}
bool JRelay::Server::isRunning() {
    return ServerRunning;
}
void JRelay::Server::setPassword(std::string Pass) {
    ServerPassword = Pass;
}

/***********************************************************************************\
|*   Server - Private
\***********************************************************************************/
void JRelay::Server::sendDataAll(std::string Data) {
    for (auto const& user : Connections) sendDataOne(user.second.id, Data);
}
void JRelay::Server::sendDataOne(int ClientID, std::string Data) {
    User user = Connections[ClientID];

    // Encrypt Data
    if (EType != Encryption::Unencrypted) Data = encryptData(Data, user.key, "");

    if (SendDataAttempts) {
        ::send(user.socket, Data.c_str(), sizeof(Data) * JBITSTOBYTES, 0);
    } else {
        for (unsigned int i = 0; i <= SendDataAttempts; i++) {
            ::send(user.socket, Data.c_str(), sizeof(Data) * JBITSTOBYTES, 0);
            
            if (SendDataDelay) std::this_thread::sleep_for(std::chrono::microseconds(SendDataDelay));
        }
    }
}

int JRelay::Server::clientMessageHandler(User user, char Message[]) {
    std::vector<DataInfo> DataContainer;
    std::string data = std::string(Message);

    // Decrypt Data
    if (EType != Encryption::Unencrypted) {
        PRINT_UPDATE("\033[1;34mCData: \033[1;32m" + data + "\033[0m\n");
        data = decryptData(data, user.key, "");
    }
    PRINT_UPDATE("\033[1;34mCData: \033[1;32m" + data + "\033[0m\n");
    
    int ind = 0;
    while (true) {
        DataInfo newdata = createDataPair(user.id, data, ind);
    
             if (newdata.variable == UNKNOWN_DATA) continue;
        else if (newdata.variable == DISCONNECT_REQUEST) { return 0; } // Any data sent before disconnect message will be handled
        else if (newdata.variable == TIMEDOUT_CHECK) CheckDisconnect[user.id] = false; // Mutex may be needed here if problems arise
        else DataContainer.push_back(newdata);

        if ((ind = (int)data.find(",", ind)) == std::string::npos) break;
        else ind++;
    }
    
    // Send Data to handler
    if (RecvDataOnThread) {
        std::thread dt = std::thread(this->DataHandler, this, user.id, DataContainer);
        dt.detach();
    } else DataHandler(this, user.id, DataContainer);

    // Return success
    return 1;
}
// TODO
// - Have client verify it's ID
// - ret = recv -> 0 send timeout check to client
void JRelay::Server::clientListenThread(int ClientID) {
    User user = Connections[ClientID];
    char client_message[JRECVSIZE];
    bool timedout = false;
    int ret;

    // Set socket to blocking
    if (!setBlocking(user.socket)) {
        ERRORLOG.push_back("Failed to set client socket to blocking\n");
        return;
    }
    
    while (ServerRunning) {
        // Receive message from client
        ret = ::recv(user.socket, client_message, JRECVSIZE, 0);
        if (ret == 0) { 
            timedout = true; 
            // TODO
            break; 
        } else if (ret == SOCKET_ERROR) {
            ERRORLOG.push_back(user.name + " recv failed. " + std::string(strerror(errno)));
            break;
        }
        
        if (!clientMessageHandler(user, client_message)) break;
    }
    
    // Remove client
    handleClientDisconnect(ClientID, timedout);
}
// TODO 
// - Test security for password
// - DDos protection
void JRelay::Server::handleNewConnection(SOCKET NewClient) {
    char resp[JRECVSIZE];
    uint64_t id;
    std::string sresp, ukey, ekey;

    // Recieve dummy data
    if (recvtimed(NewClient, 5000, resp) != 0) { 
        PRINT_UPDATE("Client connect recv error: " + std::to_string(errno));
        ERRORLOG.push_back("Client connect recv error: " + std::to_string(errno)); 
        return; 
    }
    
    // Exchange keys
    if (EType != Encryption::Unencrypted) {
        if (ASyncKeys) {

        } else {
            // Get ClientKey
            if (recvtimed(NewClient, 5000, resp) != 0) {
                PRINT_UPDATE("Failed to receive key: Error " + std::to_string(errno) + "\n");
                ERRORLOG.push_back("Failed to receive key: Error " + std::to_string(errno));
                return;
            }

            std::string clientkey = std::string(resp);
            PRINT_UPDATE("Recieved TransactionKey: " + clientkey + "\n");

            // Generate ServerKey
            initEncryption();
            ukey = PublicKey;
            PRINT_UPDATE("Created SyncKey: " + ukey + "\n");

            // Encrypt ServerKey with ClientKey
            std::string ekey = encryptData(ukey, clientkey, "");
            PRINT_UPDATE("Encrypted SyncKey: " + ekey + "\n");

            // Send encrypted ServerKey
            ::send(NewClient, ekey.c_str(), JRECVSIZE, 0);                            
        }
    }

    // Verify password
    if (ServerPassword.length() > 0) {
        if (recvtimed(NewClient, 5000, resp) != 0) {
            PRINT_UPDATE("Client failed to send password.");
            return;
        }
        
        // Security limits need to be tested here
        sresp = decryptData(std::string(resp), ukey, 0);
        if (sresp != ServerPassword) {
            ::send(NewClient, encryptData(PASSWORD_DENIED, ukey, "").c_str(), JRECVSIZE, 0);

            PRINT_UPDATE("Connection denied: Incorrect password\n");
            ERRORLOG.push_back("Connection denied: Incorrect password");
           
            return;
        }
    }

    // Wait for readiness
    // Client will either send password guess and/or RESPONSE_READY which [should] be fine
    if (recvtimed(NewClient, 5000, resp) != 0) { 
        PRINT_UPDATE("Client ready error: " + std::to_string(errno));
        ERRORLOG.push_back("Client failed to respond ready status: timedout"); 
        return; 
    }

    // Send Client success message
    ::send(NewClient, encryptData(PASSWORD_SUCCESS, ukey, "").c_str(), JRECVSIZE, 0);

    // Generate ID
    do { id = rand() % 10000000; } while(Connections.find(id) != Connections.end());

    // Add client to server
    User newuser;
    newuser.socket = NewClient;
    newuser.id = id;
    newuser.name = "Client" + std::to_string(id);
    Connections[id] = newuser;
    PRINT_UPDATE(newuser.name + " added to server!\n");

    // Send client their ID
    ::send(NewClient, (SET_CLIENTID + ":" + std::to_string(id)).c_str(), JRECVSIZE, 0);

    // Let Handler init client
    if (ClientConnectedHandler != NULL) ClientConnectedHandler(this, id);

    // Start client listen thread
    std::thread clientthread = std::thread(&Server::clientListenThread, this, id);
    clientthread.detach();

    // Start disconnect thread
    if (CheckForDisconnect) {
        std::thread dsthread = std::thread(&Server::clientDisconnectCheckThread, this, id);
        dsthread.detach();

        //PRINT_UPDATE("Detached Client " + std::to_string(id) + "chkthread\n");
    }
}
void JRelay::Server::listenForConnections() {
    while (!ServerClosed) {
        if (CanAcceptConnections) {
            if (Connections.size() <= MaxConnections) {
                // Accept new connection
                NewConnection = ::accept(NetworkSocket, (SOCKADDR*)&SocketADDR, (socklen_t*)&SALen);

                // Connection errored
                if (NewConnection == SOCKET_ERROR) {
                    PRINT_UPDATE("NetworkError: Client failed connection\n");
                    ERRORLOG.push_back("NetworkError: Client failed connection");
                // Connection success
                } else {
                    std::thread newcon = std::thread(&Server::handleNewConnection, this, NewConnection);
                    newcon.detach();
                }
            } else std::this_thread::sleep_for(std::chrono::milliseconds(100));
        } else std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}
// TODO
// - Optional check all client on one thread - Maybe break into containters of clients
void JRelay::Server::clientDisconnectCheckThread(int ClientID) {
    User user = Connections[ClientID];

    while (ServerRunning) {
        //printf("Checking client %d connection status\n", ID);

        for (int i = 0; i < DisconnectCheckAttempts; i++) {
            // Request connected status
            sendDataOne(user.socket, TIMEDOUT_CHECK + ":0");
            CheckDisconnect[ClientID] = true;

            // Wait for delay
            std::this_thread::sleep_for(std::chrono::milliseconds(1000));

            // Check if response was received
            if (!(CheckDisconnect[ClientID])) break;
        }

        if (CheckDisconnect[ClientID]) {
            //PRINT_UPDATE("Client " + std::string(ID) + " is no longer connected. Ending check thread\n");
            
            // Remove client if revc thread didn't first
            if (Connections.find(ClientID) != Connections.end()) handleClientDisconnect(ClientID, true);
            
            break;
        } else {
            //PRINT_UPDATE("Client %d is still connected\n", ClientID);
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(DisconnectCheckPeriod));
    }
}
void JRelay::Server::handleClientDisconnect(int ClientID, bool Timeout) {
    // Let program finish with client
    if (ClientDisconnectHandler != NULL) ClientDisconnectHandler(this, ClientID);

    // Remove all instance of client from server
    PRINT_UPDATE("Client " +  std::to_string(ClientID) + (Timeout ? " timed out\n" : " disconnected --- "));
	sendDataToClient(ClientID, DISCONNECT_SUCCESS + ":0");

    if (Connections.find(ClientID) != Connections.end()) {
		closesoc(Connections[ClientID].socket);
        CheckDisconnect.erase(ClientID);
        Connections.erase(ClientID);

        PRINT_UPDATE("Removed successfully\n");
    } else {
        PRINT_UPDATE("Client not found in memory. Ignoring...\n");
    }
}

