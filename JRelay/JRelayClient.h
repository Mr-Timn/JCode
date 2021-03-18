//
// Made by Jordan Hoosman
//

#ifndef __JRELAY__
    #include "JRelay.h"
#endif

#define __JRELAYCLIENT__

namespace JRelay {
    class Client;

    typedef void(*ClientDataHandle)(Client* Client, std::vector<DataInfo> Data);

    class Client : public Network {
        public:
            std::vector<char*> FoundServers;
            
            /*** Functions ***/
            // Constructor
            Client(ClientDataHandle DataFunction, Encryption Encrypt);

            // Client
            bool connect(const char Address[], int Port);
            bool connect(const char Address[], int Port, std::string Password);
            void disconnect(bool Request);
            bool sendData(std::string Data);

            // Status
            int getID();
            bool isConnected();
            std::string getPublicKey() {
                return PublicKey;
            }
            std::string getPrivateKey() {
                return PrivateKey;
            }
            std::string getNC() {
                return NETCHARS;
            }
            
            // IP
            void launchServerSearch(bool IPv4, int Port, int Threads, bool BreakOnFirst, int Speed);
            std::vector<char*> searchForOpenServers(bool NoThreads, bool IPv4, int Port, int Threads, bool BreakOnFirst, int Speed);
            std::string getIPAddress(bool IPv4);

            bool serverOpen(const char Address[], int Port, int Speed);

            virtual bool polldata() {
                FD_ZERO(&ClientFD);
                FD_SET(NetworkSocket, &ClientFD);

                int ret = select(NetworkSocket + 1, &ClientFD, NULL, NULL, &FDTimer);
                if (ret == SOCKET_ERROR) return false;
                
                if (FD_ISSET(NetworkSocket, &ClientFD)) {
                    ret = ::recv(NetworkSocket, &PollBuffer, JRECVSIZE, NULL);
                    if (ret == SOCKET_ERROR) {
                        printf("POLL SOCKET ERROR: %s\n", strerror(errno));
                        ERRORLOG.push_back("POLLING SOCKET ERROR");
                        return false;
                    }
                    
                    if (sizeof(PollBuffer) > 0) {
                        serverMessageHandler(PollBuffer);
                    }
                }
                
                return true;
            }

        private:
            /*** Variables ***/
            ClientDataHandle DataHandler;

            uint64_t ID = 0;
            bool Connected = false;
            bool DisconnectSuccess = false;
            
            std::vector<int> SearchCheck;
            bool BREAK_SEARCH = false;

            std::string ServerKey = "";

            /*** Functions ***/
            void serverMessageHandler(char Message[]);
            void serverListenThread();
            void sendDataThread(std::string Data);
            void serverSearchThread(std::string HostAddr, int Port, int Min, int Max, int Speed);
    };
}
