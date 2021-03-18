//
// Made by Jordan Hoosman
//

#ifndef __JRELAY__
    #include "JRelay.h"
#endif

bool JRelay::fileExists(std::string File) {
	struct stat buffer;
	return (stat(File.c_str(), &buffer) == 0);
}

uint64_t JRelay::rand64() {
    uint64_t r = 0;
    
    for (int i = 0; i < 64; i += RAND_MAX_WIDTH) {
        r <<= RAND_MAX_WIDTH;
        r ^= (unsigned) rand();
    }

    return r;
}
uint32_t JRelay::rand32() {
    uint32_t r = 0;
    
    for (int i = 0; i < 32; i += RAND_MAX_WIDTH) {
        r <<= RAND_MAX_WIDTH;
        r ^= (unsigned) rand();
    }

    return r;
}

bool operator==(JRelay::DataInfo DataInfo1, JRelay::DataInfo DataInfo2) {
    return  DataInfo1.variable == DataInfo2.variable &&
            DataInfo1.value == DataInfo2.value && 
            DataInfo1.sender == DataInfo2.sender;            
}
bool operator!=(JRelay::DataInfo DataInfo1, JRelay::DataInfo DataInfo2) {
    return !(DataInfo1 == DataInfo2);
}

/***********************************************************************************\
|*   Network - Public
\***********************************************************************************/
void JRelay::Network::disableUpdates() {
    PrintErrors = false;
}
SOCKET JRelay::Network::getSocket() {
    return NetworkSocket;
}
bool JRelay::Network::canPrint() {
    return PrintErrors;
}

void JRelay::Network::setSendAttempts(unsigned int Amount) {
    SendDataAttempts = Amount;
}
void JRelay::Network::enableSendOverThread() {
    SendDataOnThread = true;
}
void JRelay::Network::disableSendOverThread() {
    SendDataOnThread = false;
}
void JRelay::Network::setSendThreadDelay(unsigned int Delay) {
    SendDataDelay = Delay;
}

// DO NOT USE - Psuedo code for later improving 
void JRelay::Network::regenMessageConstants() {
    regenMessageConstants(10);
}
void JRelay::Network::regenMessageConstants(int Length) {
    std::string chars = "`1234567890-=[];',./~!@#$%^&*()_+{}|:<>?";

    TIMEDOUT_CHECK.clear();
    DISCONNECT_REQUEST.clear();
    UNKNOWN_DATA.clear();

    for (int i = 0; i < Length; i++) {
        TIMEDOUT_CHECK += chars[rand() % chars.length()];
        DISCONNECT_REQUEST += chars[rand() % chars.length()];
        DISCONNECT_REQUEST += chars[rand() % chars.length()];
        UNKNOWN_DATA += chars[rand() % chars.length()];
    }
}

void JRelay::Network::storeCache(JRelay::DataInfo Data) {
    std::string tag = "Cache" + generateRanString(12);

    CachedData.insert(std::pair<std::string, JRelay::DataInfo>(tag, Data));
}
void JRelay::Network::storeCache(std::string CacheID, JRelay::DataInfo Data) {
    CachedData.insert(std::pair<std::string, JRelay::DataInfo>(CacheID, Data));
}
void JRelay::Network::clearCache() {
    CachedData.clear();
}
void JRelay::Network::removeCache(std::string CacheID, bool All) {
    if (CachedData.size() == 0) return;

    if (All) {
        while (CachedData.find(CacheID) != CachedData.end()) CachedData.erase(CachedData.find(CacheID));
    } else {
        if (CachedData.find(CacheID) != CachedData.end()) CachedData.erase(CachedData.find(CacheID));
    }
}
void JRelay::Network::removeCache(std::string CacheID, DataInfo Data) {
    if (CachedData.size() == 0) return;
    else if (CachedData.find(CacheID) == CachedData.end()) return;
    else if (CachedData.count(CacheID) == 1) {
        removeCache(CacheID, false);
        return;
    }

    DataPairs datalist = getCache(CacheID);
    for (DataIterator dataiter = datalist.first; dataiter != datalist.second; ++dataiter) {
        if ((*dataiter).second == Data) {
            CachedData.erase(dataiter);
            
            break;
        }
    }
}
JRelay::DataPairs JRelay::Network::getCache(std::string CacheID) {
    return CachedData.equal_range(CacheID);
}
JRelay::DataInfo JRelay::Network::getCacheFirst(std::string CacheID) {
    if (CachedData.size() == 0 || CachedData.find(CacheID) == CachedData.end()) return DATAINFONULL;
    
    return CachedData.equal_range(CacheID).first->second;
} 
// DO NOT USE
JRelay::DataInfo JRelay::Network::getCacheLast(std::string CacheID) {
    if (CachedData.size() == 0 || CachedData.find(CacheID) == CachedData.end()) return DATAINFONULL;
    else if (CachedData.size() == 1) return CachedData.equal_range(CacheID).first->second;
    
    return CachedData.equal_range(CacheID).second->second;
}
std::multimap<std::string, JRelay::DataInfo> JRelay::Network::getAllCachedData() {
    return CachedData;
}

void JRelay::Network::storePCache(std::string PCacheID, std::string ID, void* Data) {
    PCache newpcache;
    newpcache.name = ID;
    newpcache.value = Data;

    PCachedData.insert(std::pair<std::string, PCache>(PCacheID, newpcache));
}
// TODO
// - Verify memory freeing
// - Auto free memory when needed
void JRelay::Network::clearPCache() {
    PCachedData.clear();
}
JRelay::PCachePair JRelay::Network::getPCache(std::string PCacheID) {
    return PCachedData.equal_range(PCacheID);
}
JRelay::PCache JRelay::Network::getPCacheFirst(std::string PCacheID) {
    return *getPCacheFirstRaw(PCacheID);
}
JRelay::PCache* JRelay::Network::getPCacheFirstRaw(std::string PCacheID) {
    if (PCachedData.size() == 0 || PCachedData.find(PCacheID) == PCachedData.end()) return &NULLPCACHE;

    return &PCachedData.equal_range(PCacheID).first->second;
}
// DO NOT USE
JRelay::PCache JRelay::Network::getPCacheLast(std::string PCacheID) {
    return NULLPCACHE;
}
void* JRelay::Network::getPCacheFirstValue(std::string PCacheID) {
    if (PCachedData.size() == 0) return NULL;

    return getPCacheFirst(PCacheID).value;
}
void* JRelay::Network::getPCacheLastValue(std::string PCacheID) {
    if (PCachedData.size() == 0) return NULL;

    return getPCacheLast(PCacheID).value;
}
std::multimap<std::string, JRelay::PCache> JRelay::Network::getAllPCachedData() {
    return PCachedData;
}

void JRelay::Network::initEncryption() {
    if (EType == Encryption::Unencrypted) return;
    srand(time(NULL));

    PublicKey.clear();
    PrivateKey.clear();
    KeyHash.clear();

    // Create keyhash file
    if (!fileExists(KEYHASHEFILE)) {
        std::ofstream keyfile(KEYHASHEFILE);
        keyfile.close();
    // Read keyhashes
    } else {   
        std::ifstream hashfile(KEYHASHEFILE);
        std::string line;
        std::vector<std::string> hashes;

        // Read all hashes
        while (hashfile >> line) {
            if (line.length() < 1 || line[0] == '#') continue;

            hashes.push_back(line);
        }

        // Update KeyHash
        if (hashes.size() > 0) {
            KeyHash = rand() % hashes.size();
            
            PRINT_UPDATE("Read new hash: " + KeyHash);
        }
    }

	switch (EType) {
        case Encryption::JEncrypt: {
            if (KeyHash.length() > 0) break;

            // Placeholder
            
            break;
        }
        case Encryption::Reverse: {
			ASyncKeys = false;
            break;
        }
        case Encryption::CaesarCipher: {
            ASyncKeys = false;
			PublicKey = NETCHARS[rand() % NETCHARS.length()];
            PrivateKey = PublicKey;

            break;
        }
        case Encryption::VignereCipher: {
            ASyncKeys = false;
			PublicKey = generateRanString(KEYLENGTH);
            PrivateKey = PublicKey;

			break;
		}

	}

    /*PublicKey = generateRanString(KEYLENGTH);
    PrivateKey = generateRanString(KEYLENGTH);

    PRINT_UPDATE("Created PublicKey:   \x1b[32m" + PublicKey + "\x1b[0m\n\n");
    PRINT_UPDATE("Created PrivateKey:  \x1b[31m" + PrivateKey + "\x1b[0m\n\n");
    std::string keychk1 = encryptData(PublicKey, PrivateKey);
    //PRINT_UPDATE("Encrypted PublicKey: \x1b[34m" + PublicKey + "\x1b[0m\n\n");
    std::string keychk2 = decryptData(PublicKey, PrivateKey);
    //PRINT_UPDATE("Decrypted PublicKey: \x1b[32m" + keychk2 + "\x1b[0m\n\n");

    // Verify keys work
    if (keychk1 != keychk2) {
        PRINT_UPDATE("NetworkError: PublicKey failed to decrypt. Blame the developer\n");
        PRINT_UPDATE("\x1b[31mDefaulting to Unencrypted\x1b[0m\n");
        EType = Encryption::Unencrypted;

        PRINT_UPDATE("Mismatch: ");
        for (int i = 0; i < KEYLENGTH; i++) {
            if (keychk1[i] != keychk2[i]) {
                PRINT_UPDATE(std::to_string(i) + " ");
            }
        }
        PRINT_UPDATE("\n");
    } else {
        PRINT_UPDATE("Successfully created keys!\n");
    }*/
}
std::string JRelay::Network::encryptData(std::string Data, std::string Key, std::string Chart) {
    std::string edata = "";

    switch (EType) {
        case Encryption::Unencrypted: {
            edata = Data;
            break;
        }
        case Encryption::JEncrypt: {
            
            break;
        }
        case Encryption::Reverse: {
			for (int i = Data.length() - 1; i > 0; i--) edata += Data[i];

            break;
        } 
        case Encryption::CaesarCipher: {
			if (Chart == "") Chart = NETCHARS;
            if (Key == "") Key = PrivateKey;

            for (int i = 0; i < Data.length(); i++) {
                int a = Chart.find(Data[i]);
                int b = Chart.find(Key[0]);
                int c = (a + b) % Chart.length();
				
                edata += Chart[c];
            }

            break;
        }
        case Encryption::VignereCipher: {
			if (Chart == "") Chart = NETCHARS;
			if (Key == "") Key = PublicKey;
            
            for (int i = 0, k = 0; i < Data.length(); i++) {
                int a = Chart.find(Data[i]);
                int b = Chart.find(Key[k++]);
                int c = (a + b) % Chart.length();

                edata += Chart[c];

                if (k > Key.length() - 1) k = 0;
                /*std::cout 
                    << i << ": " 
                    << Data[i] << "(" << a << ") \t\tto " 
                    << NETCHARS[c] << "(" << c << ") \t\tby " 
                    << Key[i] << "(" << b << ")"
                    << std::endl;*/
            }

            break;
        }
    }

    return edata;
}
std::string JRelay::Network::decryptData(std::string Data, std::string Key, std::string Chart) {
    std::string edata = "";

    switch (EType) {
        case Encryption::Unencrypted: {
            edata = Data;
            break;
        }
        case Encryption::JEncrypt: {
            
            break;
        }
        case Encryption::Reverse: {
			for (int i = Data.length() - 1; i > 0; i--) edata += Data[i];

            break;
        }
        case Encryption::CaesarCipher: {
			if (Chart == "") Chart = NETCHARS;
            if (Key == "") Key = PrivateKey;

			for (int i = 0; i < Data.length(); i++) {
				int c = Chart.find(Data[i]);
                int b = Chart.find(Key[0]);
                int a;
                if (c - b <= 0) a = (Chart.length() + (c - b)) % Chart.length();
                else a = (c - b) % Chart.length();
                
                edata += Chart[a];
			}

            break;
        }
        case Encryption::VignereCipher: {
			if (Chart == "") Chart = NETCHARS;
			if (Key == "") Key = PrivateKey;

            int k = 0;
            for (int i = 0; i < Data.length(); i++) {
                int c = Chart.find(Data[i]);
                int b = Chart.find(Key[k++]);
                int a;
                if (c - b <= 0) a = (Chart.length() + (c - b)) % Chart.length();
                else a = (c - b) % Chart.length();
                
                edata += Chart[a];

                if (k > Key.length() - 1) k = 0;
                /*std::cout 
                    << i << ": " 
                    << Data[i] << "(" << c << ") \t\tto " 
                    << NETCHARS[a] << "(" << a << ") \t\tby " 
                    << Key[i] << "(" << b << ")"
                    << std::endl;*/
            }

            break;
        }
    }

    return edata;
}

/***********************************************************************************\
|*   Network - Protected
\***********************************************************************************/
void JRelay::Network::PRINT_UPDATE(std::string Update) {
    if (PrintErrors) {
        printf("%s", Update.c_str());
    }
}

void JRelay::Network::scrambleString(std::string& Str, int N) {
    srand(time(NULL));

    int max = Str.length();
    for (int i = 0; i < N; i++) {
        // Pick random characters
        int a = rand() % max;
        int b = rand() % max;

        // Swap
        char temp = Str[a];
        Str[a] = Str[b];
        Str[b] = temp;
    }
}

std::string JRelay::generateRanString(int Length){
    return generateRanString(Length, "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ`1234567890 -=[];',./~!@#$%^&*(){}|:<>?");
}
std::string JRelay::generateRanString(int Length, std::string Source) {
    std::string ret = "";

    for (int i = 0; i < Length; i++) {
        ret += Source[rand() % Source.length()];
    }

    return ret;
}
std::string JRelay::Network::generateRanString(int Length) {
    return generateRanString(Length, JRelay::Network::KeyHash.length() == 0 ? NETCHARS : KeyHash);
}
std::string JRelay::Network::generateRanString(int Length, std::string Source) {
    std::string ret = "";

    for (int i = 0; i < Length; i++) {
        ret += Source[rand() % Source.length()];
    }

    return ret;
}
// TODO
// - Test and fix all crash possibilities
// - Better data cleaning
JRelay::DataInfo JRelay::Network::createDataPair(int Sender, std::string Data, int Index) {
    DataInfo NewData;

    // Find seperator
    int sep = (int)Data.find(":", Index);

    // No seperator
    if (sep == std::string::npos) return DATAINFONULL;

    // Grab variable
    std::string var = Data.substr(Index, sep - Index);

    // Grab value
    std::string val;
    int start = Index + (int)var.length() + 1;
    int end = (int)Data.find(",", Index);

    if (end > 0) val = Data.substr(start, end - start);
    else val = Data.substr(start);

    // Clean data
    //var.erase(std::remove(var.begin(), var.end(), '\n'), var.end());
    //var.erase(std::remove(var.begin(), var.end(), '\r'), var.end());
    
    // Generate data
    NewData.sender = Sender;
    NewData.variable = var;
    NewData.value = val;

    // Return data
    return NewData;
}

bool JRelay::Network::initNetworkSocket(SOCKET& Socket) {
    if ((Socket = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == SOCKET_ERROR) {
		ERRORLOG.push_back("NetworkError: Failed to create socket");

        return false;
    }

    return true;
}
bool JRelay::Network::setBlocking(SOCKET& Socket) {
    long arg;
    #ifdef _WIN32
        u_long iMode = 1;
        if ((arg = ioctlsocket(Socket, FIONBIO, &iMode)) < 0) return false;
    #else
        arg = fcntl(Socket, F_GETFL, NULL);
        if (arg < 0) return false;
        arg = arg & (~O_NONBLOCK);
        if (fcntl(Socket, F_SETFL, arg) < 0) return false;
    #endif

    SocketIsBlocking = true;

    return true;
}
bool JRelay::Network::setNonBlocking(SOCKET& Socket) {
    long arg;
    #ifdef _WIN32
        u_long iMode = 1;
        if ((arg = ioctlsocket(Socket, FIONBIO, &iMode)) < 0) return false;
    #else
        arg = fcntl(Socket, F_GETFL, NULL);
        if (arg < 0) {
            ERRORLOG.push_back("arg < 0: " + std::to_string(arg));
            return false;
        }
        arg |= O_NONBLOCK; 
        if (fcntl(Socket, F_SETFL, arg) < 0) {
            ERRORLOG.push_back("fnctl < 0");
            return false;
        }
    #endif

    SocketIsBlocking = false;

    return true;
}
 
int JRelay::Network::recvtimed(SOCKET& Socket, int Timeout, char resp[]) {
#ifdef __ANDROID__
    setBlocking(Socket);
#else
    if (!setNonBlocking(Socket)) {
        ERRORLOG.push_back("Failed to set nonblocking");
        return BLOCKING_ERROR;
    }

    fd_set myset;
    struct timeval tv;
    
    FD_ZERO(&myset);
    FD_SET(Socket, &myset);

    tv.tv_sec = 0;
    tv.tv_usec = Timeout * 1000;
    
    int rv = ::select(Socket + 1, &myset, NULL, NULL, &tv);
    if (rv == SOCKET_ERROR) return SOCKET_ERROR;
#endif
    int ret = ::recv(Socket, resp, JRECVSIZE, NULL);
    if (ret == SOCKET_ERROR) return SOCKET_ERROR;
    else if (ret == 0) return -2;
#ifndef __ANDROID__
    if (!setBlocking(Socket)) {
        ERRORLOG.push_back("Failed to revert to blocking");
        return BLOCKING_ERROR;
    }
#endif
    return 0;
}

void JRelay::Network::usePolling() {
    DataIsPolled = true;
}
void JRelay::Network::setPollDelay(unsigned int Microseconds) {
    FDDelayUS = Microseconds;
}
bool JRelay::Network::isBlocking() {
    return SocketIsBlocking;
}