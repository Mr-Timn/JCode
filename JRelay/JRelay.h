//
// Made by Jordan Hoosman
//

#undef UNICODE

#ifndef __JRELAY__
#define __JRELAY__

#include <map>							// ...					
#include <thread>						// ...
#include <vector>						// ...
#include <string>
#include <string.h>						// strcmp
#include <fcntl.h>						// fcntl
#include <fstream>						// ...
#include <algorithm>					// erase
#ifdef _WIN32
	#define WIN32_LEAN_AND_MEAN

	#include <atlbase.h>
	#include <winsock2.h>
	#include <ws2bth.h>
	#include <ws2tcpip.h>
	#include <windows.h>

	#define socklen_t int
	#define WORKING_BUFFER_SIZE 15000
	#define MAX_TRIES 3
	#define MALLOC(x) HeapAlloc(GetProcessHeap(), 0, (x))
	#define FREE(x) HeapFree(GetProcessHeap(), 0, (x))
	#define closesoc(x) ::closesocket(x)

	#pragma comment(lib, "IPHLPAPI.lib")
#else 
	#include <sys/stat.h>				// ...
	#include <unistd.h>					// close
	#include <arpa/inet.h> 				// inet_pton inet_ntop
	
	#include <stdio.h>
	#include <errno.h>
	#include <sys/time.h>

	#include <sys/time.h>
	#include <sys/param.h>
	#include <sys/types.h>
	#include <sys/socket.h>
	#include <sys/file.h>

	#include <netinet/in_systm.h>
	#include <netinet/in.h>
	#include <netinet/ip.h>
	#include <netinet/ip_icmp.h>
	#include <netdb.h>

	#include <poll.h>

	#ifdef __ANDROID__
		#include <android-ifaddr/ifaddr.h>
		#include <stdlib.h>
	#else // Android
		#include <ifaddrs.h>				// ifaddrs socket
	#endif

	#define SOCKET int
	#define SOCKADDR sockaddr
	#define SOCKADDR_IN sockaddr_in
	#define SOCKET_ERROR -1
	#define closesoc(x) ::close(x)
#endif

#pragma comment(lib, "Ws2_32.lib")
#pragma warning(disable:4996) 
#pragma warning(disable:4603) 

#define RECV_SIZE_1024 8192
#define RECV_SIZE_512  4096
#define RECV_SIZE_256  2048
#define RECV_SIZE_128  1024
#define RECV_SIZE_64   512
#define RECV_SIZE_32   256
#define JBITSTOBYTES 8

#define BLOCKING_ERROR -3

#define IMAX_BITS(m) ((m) / ((m) % 255 + 1) / 255 % 255 * 8 + 7 - 86 / ((m) % 255 + 12))
#define RAND_MAX_WIDTH IMAX_BITS(RAND_MAX)

#if defined(J_USERECV1024)
	#define JRECVSIZE RECV_SIZE_1024
#elif defined(J_USERECV512)
	#define JRECVSIZE RECV_SIZE_512
#elif defined(J_USERECV256)
	#define JRECVSIZE RECV_SIZE_256
#elif defined(J_USERECV128)
	#define JRECVSIZE RECV_SIZE_128
#elif defined(J_USERECV64)
	#define JRECVSIZE RECV_SIZE_64
#elif defined(J_USERECV32)
	#define JRECVSIZE RECV_SIZE_32
#else
	#define JRECVSIZE RECV_SIZE_256
#endif
#endif

namespace JRelay {
	struct DataInfo;
	struct PCache;

	typedef std::multimap<std::string, DataInfo>::iterator DataIterator;
	typedef std::multimap<std::string, PCache>::iterator PCacheIterator;
	
	typedef std::pair<std::multimap<std::string, DataInfo>::iterator, std::multimap<std::string, DataInfo>::iterator> DataPairs; 
	typedef std::pair<std::multimap<std::string, PCache>::iterator, std::multimap<std::string, PCache>::iterator> PCachePair;

	extern bool fileExists(std::string File);
	
	uint64_t rand64();
	uint32_t rand32();

	std::string generateRanString(int Length);
	std::string generateRanString(int Length, std::string Source);
	
	/***********************************************************************************\
	|*   Enums
	\***********************************************************************************/
	enum class Encryption {
		Unencrypted,

		// Sync Types
		JEncrypt,
		Reverse,
		VignereCipher,
		CaesarCipher,
		DiffieHellman,

		// ASync Types
		RSA
	};
	
	/***********************************************************************************\
	|*   Structs  
	\***********************************************************************************/
	struct DataInfo {
        unsigned int sender = 0;
        std::string variable = "";
        std::string value = "";
    };
	struct User {
		SOCKET socket = NULL;
		u_int64_t id = 0;
		std::string name = "";
		std::string key = "";
		std::string hash = "";

		//std::unordered_multiset<std::string> cacheddata;
	};
	struct PCache {
		std::string name;
		void* value;
	};
	
	/***********************************************************************************\
	|*   Classes  
	\***********************************************************************************/
	class Network {
		public:
			std::vector<std::string> ERRORLOG;
			DataInfo DATAINFONULL;
			DataInfo DATAINFOEMPTY;
			PCache NULLPCACHE;
			
			std::string SET_CLIENTID       = "()U$J*A(ID@X(@#*(@";
			std::string REQ_CLIENTID	   = "*)M#*^Y:!U#*&(A^^%";
			std::string DISCONNECT_REQUEST = "#*&+*-$DR#*($+_@*#";
			std::string DISCONNECT_SUCCESS = "$!*)#!{DS|!@##@+)_";
			std::string TIMEDOUT_CHECK     = "$=$@(@!DM+_@!#!@+#";
			std::string PASSWORD_SUCCESS   = "*()$@($PS@&*($$*#@";
			std::string PASSWORD_DENIED	   = "@!#_+$&PD*#-<#@)#?";
			std::string RESPONSE_READY     = "+_>#!@(RR>!l@(${!#";
			std::string NONNULL_DATA 	   = "&*(#!&D9d7*(@&#*(*";
			std::string UNKNOWN_DATA 	   = ")#*@(#NN_!#@#($+@_";
			
			/*** Functions ***/

			// 
			void disableUpdates();
			SOCKET getSocket();
			bool canPrint();
			
			// Communication
			void setSendAttempts(unsigned int Amount);
			void enableSendOverThread();
			void disableSendOverThread();
			void setSendThreadDelay(unsigned int Delay);

			// Polling
			void usePolling();
            void setPollDelay(unsigned int Microseconds);
			bool isBlocking();

			// Constants
			void regenMessageConstants();
			void regenMessageConstants(int Length);

			// Cache
			void storeCache(DataInfo Data);
            void storeCache(std::string CacheID, DataInfo Data);
			void removeCache(std::string CacheID, bool All);
			void removeCache(std::string CacheID, DataInfo Data);
			void clearCache();
			DataPairs getCache(std::string CacheID);
			DataInfo getCacheFirst(std::string CacheID); // Oldest CacheID inserted
			DataInfo getCacheLast(std::string CacheID); // Newest CacheID inserted
			std::multimap<std::string, DataInfo> getAllCachedData();
			
			// PCache
			void storePCache(std::string PCacheID, std::string ID, void* Data);
			void clearPCache();
			PCachePair getPCache(std::string PCacheID);
			PCache getPCacheFirst(std::string PCacheID);
			PCache* getPCacheFirstRaw(std::string PCacheID);
			PCache getPCacheLast(std::string PCacheID);
			void* getPCacheFirstValue(std::string PCacheID);
			void* getPCacheLastValue(std::string PCacheID);
			std::multimap<std::string, PCache> getAllPCachedData();
			
			// Encrpytion
			void initEncryption();
			std::string encryptData(std::string Data, std::string Chart, std::string Key);
			std::string decryptData(std::string Data, std::string Chart, std::string Key);
		protected:
			// Constants
			const std::string NETCHARS = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ`1234567890 -=[];',./~!@#$%^&*(){}|:<>?";
			const int REQDIS = -999;
			const std::string KEYHASHEFILE = ".keyhash.txt";

			// Sockets
			SOCKET NetworkSocket;
			SOCKADDR_IN SocketADDR; int SALen = sizeof(SocketADDR);
			bool PrintErrors = true;
			#ifdef _WIN32
				WSADATA WSAData;
				WORD DLLVersion = MAKEWORD(2, 1);
			#endif

			// Communication
			unsigned int SendDataAttempts = 1;
			unsigned int SendDataDelay = 0; // Microseconds - Delay between send attempts
			bool SendDataOnThread = false;
			bool RecvDataOnThread = false;
			bool DataIsPolled = false;
			
			// Cache
			std::multimap<std::string, DataInfo> CachedData;
			std::multimap<std::string, PCache> PCachedData;

			// Encrpytion
			Encryption EType;
            const int KEYLENGTH = JRECVSIZE;
			bool ASyncKeys = false;
            std::string PublicKey = "";
            std::string PrivateKey = "";
			std::string KeyHash = "";

			// Polling
			char PollBuffer[JRECVSIZE];
            fd_set ClientFD;
            struct timeval FDTimer;
            int FDDelayUS = 1000;
			bool SocketIsBlocking = false;

			/*** Functions ***/
			void PRINT_UPDATE(std::string Update);

			void scrambleString(std::string& Str, int N);
			std::string generateRanString(int Length);
			std::string generateRanString(int Length, std::string Map);
			DataInfo createDataPair(int Sender, std::string Data, int Index);

			bool initNetworkSocket(SOCKET& Socket);
			bool setBlocking(SOCKET& Socket);
			bool setNonBlocking(SOCKET& Socket);
			int recvtimed(SOCKET& Socket, int Timeout, char resp[]);

			virtual bool polldata() {}
	};
}
