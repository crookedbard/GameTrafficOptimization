#pragma once
#include "common.h"
#include "buffer.h"

void CalculateMD5(char *, int, char *);
class CTools
{
public:
	//static char* getmacaddress(); // doesnt work
	static char* md5buffer(CBuffer*buff);
	static char* md5string(std::string str);
	static unsigned int ip2uint(char*ip);
	static char* uint2ip(unsigned int ip);
	static bool netconnected();
	static void encryptedbuffer(CBuffer*buff, std::string key);
	static bool comparematch(char*string, char*wild);
	static unsigned int adler32(CBuffer*buff);
#if (CC_TARGET_PLATFORM != CC_PLATFORM_WIN32)
    static std::vector<std::string> getIPAddresses();
#endif
};
