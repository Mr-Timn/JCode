//
// Made by Jordan Hoosman
//

#ifndef __JRELAY__
    #include "JRelay.h"
#endif
#include <unordered_map>    // ...

#define __JRELAYSERVER__

namespace JRelay {
    class Server;

    typedef void(*ServerDataHandle)(Server* Host, int ClientID, std::vector<DataInfo> Data);
    typedef void(*ClientConnectedHandle)(Server* Host, int ClientID);
    typedef void(*ClientDisconnectedHandle)(Server* Host, int ClientID);

    class Server : public Network {
        public:
            std::unordered_map<int, User> Connections;

            /*** Functions ***/
            
            // Constructor
            Server(int DesiredPort, int MaxConnections, ServerDataHandle DataFunction);
            Server(int DesiredPort, int MaxConnections, ServerDataHandle DataFunction, Encryption Encrypt);

            // Handles
            void setClientInitFunction(ClientConnectedHandle InitFunction);
            void setClientDiscFunction(ClientDisconnectedHandle DiscFunction);
            
            // Server
            bool start();
            void close();
            void turnoff();
            void turnon();
            void waitForTurnOff(int Timeout);
            void setPassword(std::string Pass);
            void sendDataToClient(int ClientID, std::string Data);
            void sendDataToAllClients(std::string Data);
            bool hasMaxConnections();
            int numConnected();
            void enableDisconnectCheck(int Period);

            void removeClient(int ClientID);
            
            // Status
            bool isOpen();
            bool isRunning();

            // NOT a speed efficent method with high volume of clients
            // TODO - Fix that
            // - Note: New connection handle is still threaded. Might require fix
            virtual bool polldata() {
                // Accept pending connections
                FD_ZERO(&ClientFD);
                FD_SET(NetworkSocket, &ClientFD);

                int ret = ::select(NetworkSocket + 1, &ClientFD, NULL, NULL, &FDTimer);
                if (ret == SOCKET_ERROR) return false;

                if (FD_ISSET(NetworkSocket, &ClientFD)) {
                    NewConnection = ::accept(NetworkSocket, (SOCKADDR*)&SocketADDR, (socklen_t*)&SALen);
                    if (NewConnection != SOCKET_ERROR) {
                        // Handle connection
                        std::thread newcon = std::thread(&Server::handleNewConnection, this, NewConnection);
                        newcon.detach();
                    }
                }

                // Check for client data
                for (const auto& user : Connections) {
                    FD_ZERO(&ClientFD);
                    FD_SET(user.second.socket, &ClientFD);

                    int ret = ::select(user.second.socket + 1, &ClientFD, NULL, NULL, &FDTimer);
                    if (ret == SOCKET_ERROR) return false;

                    if (FD_ISSET(user.second.socket, &ClientFD)) {
                        ret = ::recv(user.second.socket, &PollBuffer, JRECVSIZE, NULL);
                        if (ret == SOCKET_ERROR) {
                            handleClientDisconnect(user.second.id, false);
                        }
                        
                        if (sizeof(PollBuffer) > 0) {
                            clientMessageHandler(user.second, PollBuffer);
                        }
                    }
                }
                
                return true;
            }

        private:
            /*** Variables ***/

            // Client communication
            SOCKET NewConnection;
            bool CheckForDisconnect = false;
            int DisconnectCheckPeriod = 0;
            int DisconnectCheckAttempts = 3;
            std::unordered_map<int, bool> CheckDisconnect;

            // Control
            bool ServerRunning = false; // Dumps all running threads
            bool ServerClosed = false; // Closes server
            bool CanAcceptConnections = false;
            unsigned int MaxConnections = 0;
            unsigned int Port = 0;
            

            // Security
            bool ServerEncrypted = false;
            std::string ServerPassword = "";
            
            // Program handles
            ServerDataHandle DataHandler = NULL;
            ClientConnectedHandle ClientConnectedHandler = NULL;
            ClientDisconnectedHandle ClientDisconnectHandler = NULL;

            /*** Functions ***/
            void sendDataAll(std::string Data);
            void sendDataOne(int ClientID, std::string Data);
            
            int clientMessageHandler(User user, char Message[]);
            void clientListenThread(int ClientID);
            void handleNewConnection(SOCKET NewClient);
            void listenForConnections();

            void clientDisconnectCheckThread(int ID);
            void handleClientDisconnect(int ClientID, bool Timeout);
    };
}
