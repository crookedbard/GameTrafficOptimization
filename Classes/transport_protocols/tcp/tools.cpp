#include "tools.h"

/*char* CTools::getmacaddress()//http://www.codeguru.com/Cpp/I-N/network/networkinformation/article.php/c5451
{
	static char retval[20];
	IP_ADAPTER_INFO AdapterInfo[16];
	DWORD dwBufLen = sizeof(AdapterInfo);
	DWORD dwStatus = GetAdaptersInfo(AdapterInfo, &dwBufLen);
	assert(dwStatus == ERROR_SUCCESS);
	PIP_ADAPTER_INFO pAdapterInfo = AdapterInfo;
	if (pAdapterInfo)
	{
		sprintf(retval, "%02X-%02X-%02X-%02X-%02X-%02X", AdapterInfo->Address[0], AdapterInfo->Address[1], AdapterInfo->Address[2], AdapterInfo->Address[3], AdapterInfo->Address[4], AdapterInfo->Address[5]);
	}
	else retval[0] = '\0';
	return retval;
}*/

char* CTools::md5buffer(CBuffer *buff)
{
	static char a[33];
	CalculateMD5(buff->data, buff->count, a);
	return a;
}

char* CTools::md5string(std::string str)
{
	char *txtChar = new char[str.size() + 1];
	txtChar[str.size()] = 0;
	memcpy(txtChar, str.c_str(), str.size());

	static char a[33];
	//CalculateMD5(str, (int)strlen(str), a);
	CalculateMD5(txtChar, str.length(), a);
	return a;
}

unsigned int CTools::ip2uint(char *ip)
{
	return inet_addr(ip);
}

char* CTools::uint2ip(unsigned int ip)
{
	in_addr a;
	a.s_addr = (u_long)ip;
	return inet_ntoa(a);
}

bool CTools::netconnected()
{
	//Another NO-OP function. TODO: Figure out for Linux.
	return true;
}

void CTools::encryptedbuffer(CBuffer *buff, std::string key)
{
	char *txtChar = new char[key.size() + 1];
	txtChar[key.size()] = 0;
	memcpy(txtChar, key.c_str(), key.size());

	char *inp = buff->data;
	unsigned int inplen = buff->count;
	char KeyBox[257];
	int keylen = std::min((int)strlen(txtChar), 256);
	if (keylen <= 0)return;
	unsigned long i, j, t, x;
	char temp;
	i = j = t = 0;
	for (i = 0; i < 256; i++)
		KeyBox[i] = (char)i;
	for (i = 0; i < 256; i++)
	{
		j = (j + (unsigned long)KeyBox[i] + key[i % keylen]) % 256;
		temp = KeyBox[i];
		KeyBox[i] = KeyBox[j];
		KeyBox[j] = temp;
	}
	i = j = 0;
	for (x = 0; x < inplen; x++)
	{
		i = (i + 1U) % 256;
		j = (j + (unsigned long)KeyBox[i]) % 256;
		temp = KeyBox[i];
		KeyBox[i] = KeyBox[j];
		KeyBox[j] = temp;
		t = ((unsigned long)KeyBox[i] + (unsigned long)KeyBox[j]) % 256;
		inp[x] = (inp[x] ^ KeyBox[t]);
	}
}

bool CTools::comparematch(char*string, char*wild)
{
	char *cp = 0, *mp = 0;
	while ((*string) && (*wild != '*')) {
		if ((*wild != *string) && (*wild != '?')) {
			return 0;
		}
		wild++;
		string++;
	}
	while (*string) {
		if (*wild == '*') {
			if (!*++wild) {
				return 1;
			}
			mp = wild;
			cp = string + 1;
		}
		else if ((*wild == *string) || (*wild == '?')) {
			wild++;
			string++;
		}
		else {
			wild = mp;
			string = cp++;
		}
	}
	while (*wild == '*') {
		wild++;
	}
	return !*wild;
}
unsigned int CTools::adler32(CBuffer*buff)
{
	char*data = buff->data;
	unsigned int len = buff->count;
	unsigned int a = 1, b = 0;
	while (len) {
		unsigned tlen = len > 5550 ? 5550 : len;
		len -= tlen;
		do {
			a += *data++;
			b += a;
		} while (--tlen);
		a = (a & 0xffff) + (a >> 16) * (65536 - 65521);
		b = (b & 0xffff) + (b >> 16) * (65536 - 65521);
	}
	if (a >= 65521)
		a -= 65521;
	b = (b & 0xffff) + (b >> 16) * (65536 - 65521);
	if (b >= 65521)
		b -= 65521;
	return b << 16 | a;
}

// Get a Vector of the IP addresses of this computer
std::vector<std::string> CTools::getIPAddresses() {
    
    std::vector<std::string> IPAddresses;
    int TempAddress = 0;
    char* TempIPAddress = (char*) malloc(16);
    
    ifaddrs* ifap = NULL;   //Head of the interface address linked list
    
    if (getifaddrs(&ifap) == 0 && ifap != NULL) {
        
        //Get another pointer to move down the linked list
        ifaddrs* current = ifap;
        
        //Move down the linked list until we get to a NULL pointer
        while (current != NULL) {
            
            //Create a pointer of the correct type to work with
            const sockaddr_in* interfaceAddress = reinterpret_cast<const sockaddr_in*>(current->ifa_addr);
            
            if (current->ifa_addr != NULL) {
                if (current->ifa_addr->sa_family == AF_INET) {
                    //printf("%s:", current->ifa_name);
                    if (interfaceAddress != NULL) {
                        TempAddress = ntohl(interfaceAddress->sin_addr.s_addr);
                        sprintf(TempIPAddress, "%d.%d.%d.%d", (TempAddress >> 24) & 0xFF, (TempAddress >> 16) & 0xFF,
                                (TempAddress >> 8) & 0xFF, TempAddress & 0xFF);
                        //Don't include the lookback address
                        if (strcmp(TempIPAddress, "127.0.0.1") != 0) {
                            IPAddresses.push_back(std::string(TempIPAddress));
                        }
                        //printf("%s\n", TempIPAddress);
                    }
                }
            }
            
            //Move to the next node in the linked-list
            current = current->ifa_next;
        }
        
        //Release the interface memory
        freeifaddrs(ifap);
        ifap = NULL;
    }
    
    return IPAddresses;
    
}
