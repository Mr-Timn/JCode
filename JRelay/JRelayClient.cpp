//
// Made by Jordan Hoosman
//

#ifndef __JRELAYCLIENT__
    #include "JRelayClient.h"
#endif

/***********************************************************************************\
|*   Client - Public
\***********************************************************************************/
JRelay::Client::Client(ClientDataHandle DataFunction, Encryption Encrypt) {
    #ifdef _WIN32
        if (WSAStartup(DLLVersion, &WSAData) != 0) {
            PRINT_UPDATE("NetworkError: WSA Failed to start\n");
        }
    #endif
    
    DataHandler = DataFunction;
    EType = Encrypt;
	
	DATAINFONULL.sender = 0;
	DATAINFONULL.value = UNKNOWN_DATA;
	DATAINFONULL.variable = UNKNOWN_DATA;

    DATAINFOEMPTY.sender = 0;
    DATAINFOEMPTY.variable = NONNULL_DATA;
    DATAINFOEMPTY.value = NONNULL_DATA;

    NULLPCACHE.name = UNKNOWN_DATA;
    NULLPCACHE.value = NULL;
}

// TODO
// - Restructure to support polling
bool JRelay::Client::connect(const char Address[], int Port) {
    return connect(Address, Port, "");
}
bool JRelay::Client::connect(const char Address[], int Port, std::string Password) {
    if (sizeof(Address) == 0) return false;
	else if (strcmp(Address, "localhost") == 0) Address = getIPAddress(true).c_str(); //Address = "127.0.0.1";

    if (!initNetworkSocket(NetworkSocket)) return false;
    
    // Set up connection Address/Port
    SocketADDR.sin_port = htons(Port);
    SocketADDR.sin_family = AF_INET;
    #ifdef _WIN32
        SocketADDR.sin_addr.s_addr = inet_addr(Address);
    #else
        inet_pton(AF_INET, Address, &SocketADDR.sin_addr);
    #endif
    
    // Check if server is open
	#ifndef __ANDROID__
    if (!serverOpen(Address, Port, 3000)) {
		ERRORLOG.push_back("Server isn't open");
		return false;
	}
	#endif

    // Set blocking
    if (!setBlocking(NetworkSocket)) {
        ERRORLOG.push_back("Failed to set blocking");
        return false;
    }
    
    int ret = ::connect(NetworkSocket, (SOCKADDR*)&SocketADDR, SALen);
    if (ret == SOCKET_ERROR) {
        ERRORLOG.push_back("NetworkError: Client failed to connect - " + std::string(strerror(errno)));
		
		return false;
    } else {
        char resp[JRECVSIZE];
        ::send(NetworkSocket, RESPONSE_READY.c_str(), JRECVSIZE, 0);

        if (EType != Encryption::Unencrypted) {
            if (ASyncKeys) {

            } else {
                // Generate ClientKey
                initEncryption();
                std::string clientkey = PublicKey;
                PRINT_UPDATE("ResponseKey: " + clientkey + "\n");

                // Send ClientKey
                ::send(NetworkSocket, clientkey.c_str(), JRECVSIZE, 0);

                // Receive ecrypted ServerKey
                if (recvtimed(NetworkSocket, 5000, resp) != 0) {
                    PRINT_UPDATE("Server failed to send key: Timed out\n");
                    ERRORLOG.push_back("Server failed to send key: Timed out");
                    return false;
                }
                std::string synckey = std::string(resp);
                PRINT_UPDATE("Encrypted SyncKey: " + synckey + "\n");

                // Decrypt ServerKey
                PublicKey = decryptData(synckey, clientkey, "");
                PRINT_UPDATE("Decrypted SyncKey: " + PublicKey + "\n");
            }    
        }

        // Send password
        if (Password != "") ::send(NetworkSocket, Password.c_str(), JRECVSIZE, 0);
        
        // Ready for status response - This can be anything
        ::send(NetworkSocket, RESPONSE_READY.c_str(), JRECVSIZE, 0);

        // Receive status response
        if ((ret = recvtimed(NetworkSocket, 5000, resp)) != 0) {
            if (ret == SOCKET_ERROR) ERRORLOG.push_back("Server failed to send response status: " + std::string(strerror(errno)));
			else if (ret == BLOCKING_ERROR) ERRORLOG.push_back("Recv returned block error: " + std::string(strerror(errno)));

            return false;
        }
        std::string sresp = std::string(resp);

        // Decrypt response and verify status
        std::string status = decryptData(sresp, PublicKey, "");
        //PRINT_UPDATE("Server responded with: " + status + "\n");
        if (status != PASSWORD_SUCCESS) {
            PRINT_UPDATE("Incorrect password\n");
			ERRORLOG.push_back("Server failed to send key: Timed out");
            return false;
        } else {
            PRINT_UPDATE("Password accepted!\n");
        }
		
        Connected = true;
        
        if (DataIsPolled) {
            if (!setNonBlocking(NetworkSocket)) {
                ERRORLOG.push_back("Failed to set non-blocking");
                return false;
            }

            FD_ZERO(&ClientFD);
            FD_SET(NetworkSocket, &ClientFD);

            FDTimer.tv_sec = 0;
            FDTimer.tv_usec = FDDelayUS;
        } else {
            // Detach listen thread
            std::thread listenthread = std::thread(&Client::serverListenThread, this);
            listenthread.detach();
        }
        
        return true;
    }
}
void JRelay::Client::disconnect(bool Request) {
    SendDataOnThread = false;
    Connected = false;

    // Attempt disconnect request until successful
    int attempts = 0;
    int maxattempts = 5;
    if (Request){
        do {
            sendData(DISCONNECT_REQUEST + ":0");
            
            std::this_thread::sleep_for(std::chrono::milliseconds(50));
        } while (!DisconnectSuccess && attempts++ <= maxattempts);
    }

    #ifdef _WIN32
        ::closesocket(NetworkSocket);
    #else
        ::close(NetworkSocket);
    #endif

    PRINT_UPDATE("Disconnected successfully\n");
}	

bool JRelay::Client::sendData(std::string Data) {
    if (SendDataOnThread) {
        std::thread th = std::thread(&Client::sendDataThread, this, Data);
        th.detach();
    } else {
        sendDataThread(Data);
    }

    return true;
}

int JRelay::Client::getID() {
    return ID;
}
bool JRelay::Client::isConnected() {
    return Connected;
}
void JRelay::Client::launchServerSearch(bool IPv4, int Port, int Threads, bool BreakOnFirst, int Speed) {
    std::thread lss = std::thread(&JRelay::Client::searchForOpenServers, this, true, IPv4, Port, Threads, BreakOnFirst, Speed);
    lss.detach();
}
std::vector<char*> JRelay::Client::searchForOpenServers(bool NoThreads, bool IPv4, int Port, int Threads, bool BreakOnFirst, int Speed) {
    // Clear container
    FoundServers.clear();
    
    // Find ip address
    std::string ipaddr = getIPAddress(IPv4); //printf("IPAddr: %s\n", ipaddr.c_str());
    if (ipaddr.length() < 1 || ipaddr == "0.0.0.0") return FoundServers;

    // Get host address
    std::string hostaddr = ipaddr.substr(0, ipaddr.find_last_of(".")); //printf("HostAddr: %s\n", hostaddr);

    int ratiomin;
    int ratiomax;
    for (int i = 1; i <= Threads; i++) {
        ratiomin = (255 / Threads) * (i - 1) + 1;
        ratiomax = (255 / Threads) * i;

        //std::cout << "Launching " << ratiomin << " - " << ratiomax << std::endl; 

        if (NoThreads) {
            serverSearchThread(hostaddr, Port, 0, 255, Speed); break;
        } else {
            std::thread findserver = std::thread(&Client::serverSearchThread, this, hostaddr, Port, ratiomin, ratiomax, Speed);
            findserver.detach();
        }

        SearchCheck.push_back(false);
    }
    // Thread for any missed ips
    //std::cout << "Launching " << ratiomax << " - 255"  << std::endl; 
    if (!NoThreads){
        std::thread findserver = std::thread(&Client::serverSearchThread, this, hostaddr, Port, ratiomax, 255, Speed);
        findserver.detach();
    }

    SearchCheck.push_back(false);
    
    while (!NoThreads) {
        if (SearchCheck.size() == 0 || (BreakOnFirst && FoundServers.size() > 0)){
            if (BreakOnFirst) {
                BREAK_SEARCH = true;
                
                printf("Waiting for threads to end...\n");

                //std::string thleft;
                //printf("Threads left: ");
                
                std::chrono::time_point<std::chrono::system_clock> TimeLast, TimeNow;
                std::chrono::duration<double> Delta;
                do {
                    //thleft = std::to_string(SearchCheck.size());
                    //printf("%s", thleft.c_str());

                    std::this_thread::sleep_for(std::chrono::milliseconds(10));

                    TimeNow = std::chrono::system_clock::now();
                    Delta = TimeNow - TimeLast;

                    if (Delta.count() > 1000 * 2) break;

                    //for (int i = 0; i < thleft.length(); i++) printf("\b \b"); 
                } while (SearchCheck.size() != 0);

                printf("\nEnded all search threads successfully\n");
            }

            //printf("All IP Addresses were searched!\n");
            
            break;
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(250));
    }

    return FoundServers;
}

std::string JRelay::Client::getIPAddress(bool IPv4) {
    #if defined(_WIN32)
        system(("ipconfig | findstr \"" + std::string(IPv4 ? "IPv4" : "IPv6") + "\" > tempnsip.txt").c_str());

        if (fileExists("tempnsip.txt")) {
            std::ifstream ipfile;
            ipfile.open("tempnsip.txt");

            std::string line;
            while (getline(ipfile, line)) {
                // Get ip address
                // - Could cause problems later if windows 10 wants to be dumb
                std::string ip = line.substr(line.find(":") + 2);

                // Close file so we can remove it
                ipfile.close();

                // Remove file
                remove("tempnsip.txt");

                return ip;
            }

            remove("tempnsip.txt");
        } else {
            PRINT_UPDATE("Can't get IP Address - Failed to create file\n");

            return "0.0.0.0";
        }
    #else
        struct ifaddrs* ifAddrStruct = NULL;
        struct ifaddrs* ifa = NULL;
        void* tmpAddrPtr = NULL;
        std::string retip = "";

        getifaddrs(&ifAddrStruct);

        for (ifa = ifAddrStruct; ifa != NULL; ifa = ifa->ifa_next) {
            if (!ifa->ifa_addr) continue;

            // Verify IPv4
            if (IPv4 && ifa->ifa_addr->sa_family == AF_INET) {
                tmpAddrPtr = &((struct sockaddr_in*)ifa->ifa_addr)->sin_addr;
                char addressBuffer[INET_ADDRSTRLEN];
                inet_ntop(AF_INET, tmpAddrPtr, addressBuffer, INET_ADDRSTRLEN);

                //printf("%s IP Address %s\n", ifa->ifa_name, addressBuffer);
                if (strcmp(addressBuffer, "127.0.0.1") != 0 
                    && (/*(strcmp(ifa->ifa_name, "virbr0") == 0 ||*/ strcmp(ifa->ifa_name, "enp9s0") == 0 || strcmp(ifa->ifa_name, "lo") == 0 || strcmp(ifa->ifa_name, "wlan0") == 0 )
                    ) {
                        retip = std::string(addressBuffer);
                        break;
                    }
            
            // Verify IPv6 
            } else if (!IPv4 && ifa->ifa_addr->sa_family == AF_INET6) {
                tmpAddrPtr = &((struct sockaddr_in6*)ifa->ifa_addr)->sin6_addr;
                char addressBuffer[INET6_ADDRSTRLEN];
                inet_ntop(AF_INET6, tmpAddrPtr, addressBuffer, INET6_ADDRSTRLEN);

                //printf("%s IP Address %s\n", ifa->ifa_name, addressBuffer); 
                if (!IPv4 && (
                    /*strcmp(ifa->ifa_name, "virbr0") == 0 ||*/ strcmp(ifa->ifa_name, "lo") == 0 || strcmp(ifa->ifa_name, "wlan0") == 0)
					) {
                    retip = std::string(addressBuffer);
					break;
					}
            }
        }

        if (ifAddrStruct != NULL) freeifaddrs(ifAddrStruct);
        if (retip.length() == 0) return "0.0.0.0";

        return retip;
    #endif
}

/***********************************************************************************\
|*   Client - Private
\***********************************************************************************/
void JRelay::Client::serverMessageHandler(char Message[]) {
    int ret, ind;
    std::vector<DataInfo> DataContainer;
    std::string data = std::string(Message);
    
    // Decrypt data
    if (EType != Encryption::Unencrypted) {
        PRINT_UPDATE("\033[1;31mSData: \033[1;32m" + data + "\033[0m\n");
        data = decryptData(data, PublicKey, "");
    }
    PRINT_UPDATE("\033[1;31mSData: \033[1;32m" + data + "\033[0m\n");

    ind = 0;
    while (!DisconnectSuccess) {
        DataInfo newdata = createDataPair(0, data, ind);

             if (newdata.variable == UNKNOWN_DATA) continue; 
        else if (newdata.variable == DISCONNECT_SUCCESS) DisconnectSuccess = true;
        else if (newdata.variable == TIMEDOUT_CHECK) sendData(TIMEDOUT_CHECK + ":0");
        else if (newdata.variable == SET_CLIENTID) ID = std::stoull(newdata.value);
        else DataContainer.push_back(newdata);

        if ((ind = (int)data.find(",", ind)) == std::string::npos) break;
        else ind++;
    }

    // Send DataContainer to handler
    if (RecvDataOnThread) {
        std::thread dt = std::thread(DataHandler, this, DataContainer);
        dt.detach();
    } else DataHandler(this, DataContainer);
}
void JRelay::Client::serverListenThread() {
    char server_message[JRECVSIZE];
    int ret, ind;
    
    setBlocking(NetworkSocket);

    while (Connected) {
        // Receive message
        ret = ::recv(NetworkSocket, server_message, JRECVSIZE, 0);
        if (ret == 0 || ret == SOCKET_ERROR) break;
        
        serverMessageHandler(server_message);
    }
    
    disconnect(false);
}
void JRelay::Client::sendDataThread(std::string Data) {
    if (ID == 0) {
        //::send(NetworkSocket, (REQ_CLIENTID + ":0").c_str(), JRECVSIZE, 0);
        return;
    }

	// Encrypt data
	if (EType != Encryption::Unencrypted) Data = encryptData(Data, PublicKey, "");

    if (SendDataAttempts) {
        for (unsigned int i = 0; i <= SendDataAttempts; i++) {
            ::send(NetworkSocket, Data.c_str(), sizeof(Data) * JBITSTOBYTES, 0);

            if (SendDataDelay) std::this_thread::sleep_for(std::chrono::microseconds(SendDataDelay));
        }
    } else {
        ::send(NetworkSocket, Data.c_str(), sizeof(Data) * JBITSTOBYTES, 0);
    }
}
void JRelay::Client::serverSearchThread(std::string HostAddr, int Port, int Min, int Max, int Speed) {
    #ifdef _WIN32
        if (WSAStartup(DLLVersion, &WSAData) == SOCKET_ERROR) {
            printf("Error initialising WSA.\n");
            return;
        }
    #endif
        
    SOCKET soc;
    int res; 
    struct sockaddr_in addr; 
    fd_set myset; 
    struct timeval tv; 
    int valopt; 
    socklen_t lon; 

    for (int i = Min; !BREAK_SEARCH && i < Max; i++) {
        std::string tempip = HostAddr + "." + std::to_string(i);
        // If uncommented
        // - Add #include <mutex>
        // - Add std::mutex SearchLock;
        //SearchLock.lock();
        //ERRORLOG.push_back(tempip);
        //SearchLock.unlock();
        
        if (serverOpen(tempip.c_str(), Port, Speed)) {
            PRINT_UPDATE("FOUND SERVER: " + tempip);

            FoundServers.push_back(const_cast<char*>(tempip.c_str())); 
        }
    }
    
    //printf("s \b \b"); // Help the threads don't end without this - Whyyyyy
    SearchCheck.pop_back(); 
}

bool JRelay::Client::serverOpen(const char Address[], int Port, int Speed) {
    #ifdef _WIN32
        if (WSAStartup(DLLVersion, &WSAData) == SOCKET_ERROR) {
            printf("Error initialising WSA.\n");
            return false;
        }
    #endif
     
    SOCKET soc;
    int res, valopt; 
    struct sockaddr_in addr; 
    struct timeval tv; 
    fd_set myset; 
    socklen_t lon; 

    if (!initNetworkSocket(soc)) {
        ERRORLOG.push_back("Failed to init test connection socket");
        return false;
    }

    addr.sin_family = AF_INET; 
    addr.sin_port = htons(Port); 
    #ifdef _WIN32
        addr.sin_addr.s_addr = inet_addr(Address);
    #else
        inet_pton(AF_INET, Address, &addr.sin_addr);
    #endif

    if (!setNonBlocking(soc)) {
        ERRORLOG.push_back("Failed to set to nonblocking");
        return false;
    }

    // Trying to connect with timeout 
    #ifdef _WIN32
        res = ::connect(soc, (SOCKADDR*)& addr, sizeof(addr));
    #else
        res = ::connect(soc, (struct sockaddr*)& addr, sizeof(addr));
    #endif
    if (res < 0) { 
        if (errno == EINPROGRESS || errno == 0) { 
            tv.tv_sec = 0; 
            tv.tv_usec = Speed * 1000; 
            FD_ZERO(&myset); 
            FD_SET(soc, &myset); 

            res = ::select(soc + 1, NULL, &myset, NULL, &tv); 
            if (res < 0 && errno != EINTR) { 
				ERRORLOG.push_back("errno != EINTR && res = " + std::to_string(res));
            } else if (res > 0) { 
                lon = sizeof(int);
                
				#ifdef _WIN32
                    if (getsockopt(soc, SOL_SOCKET, SO_ERROR, (char*)(&valopt), &lon) < 0) {
                        ERRORLOG.push_back("getsockopt failed");
                        return false;
                    } 
                #else
                if (getsockopt(soc, SOL_SOCKET, SO_ERROR, (void*)(&valopt), &lon) < 0) {
                     ERRORLOG.push_back("getsockopt failed");
                } 
                #endif
				// Success
				if (!valopt) {
					closesoc(soc);
					return true; 
				} 
				else ERRORLOG.push_back("optval resulted in " + std::to_string(valopt));
			} 
			else ERRORLOG.push_back("Select failed errno: " + std::string(strerror(errno)));
        } 
		else ERRORLOG.push_back("Bad errno value: " + std::string(strerror(errno)));
    } 
	else ERRORLOG.push_back("Failed to connect res = " + std::to_string(res));
	
	// Failed
    closesoc(soc);
    return false;
}

