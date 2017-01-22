#include "socket.h"
#if (CC_TARGET_PLATFORM != CC_PLATFORM_IOS && CC_TARGET_PLATFORM != CC_PLATFORM_WIN32)
#include "RohcCompression.hpp"
#include <rohc/rohc.h> /* includes required to use the compression part of the ROHC library */
#include <rohc/rohc_buf.h>  /* for the rohc_buf_*() functions */
#include <rohc/rohc_comp.h>   /* for rohc_comp_*() functions */
#include <rohc/rohc_decomp.h> /* for rohc_decomp_*() functions */
#endif

#if defined( __WIN32__ ) || defined( WIN32 ) || defined( _WIN32 )
int SenderAddrSize = sizeof(SOCKADDR_IN);
SOCKADDR_IN CSocket::SenderAddr;
//int sockid=-1;
#else
	#define TCP_NODELAY 0x0001
	#define INVALID_SOCKET -1
	#define SOCKET_ERROR   -1
	#define closesocket(s) close(s);
	int SenderAddrSize = sizeof(sockaddr);
	sockaddr_in CSocket::SenderAddr;
	int CSocket::sockid=-1;
//#include <android/log.h>
#endif


//good
#if defined( __WIN32__ ) || defined( WIN32 ) || defined( _WIN32 )
bool CSocket::tcpconnect(char* address, int port, int mode)
{
	SOCKADDR_IN addr;
	LPHOSTENT hostEntry;
	if ((sockid = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == SOCKET_ERROR)
		return false;
	if ((hostEntry = gethostbyname(address)) == nullptr)
	{
		closesocket(sockid);
		return false;
	}
	addr.sin_family = AF_INET;
	addr.sin_addr = *reinterpret_cast<LPIN_ADDR>(*hostEntry->h_addr_list);
	addr.sin_port = htons(static_cast<u_short>(port));
	if (mode == 2)setsync(1);
	if (connect(sockid, reinterpret_cast<LPSOCKADDR>(&addr), sizeof(SOCKADDR_IN)) == SOCKET_ERROR)
	{
		if (WSAGetLastError() != WSAEWOULDBLOCK)
		{
			closesocket(sockid);
			return false;
		}
	}
	if (mode == 1)setsync(1);
	return true;
}
#else
	bool CSocket::tcpconnect(char *address, int port, int mode)
	{
		sockaddr_in addr;
		hostent* hostEntry;
		if((sockid = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == SOCKET_ERROR)
			return false;
		if((hostEntry = gethostbyname(address)) == NULL)
		{
			closesocket(sockid);
			return false;
		}
//std::stringstream ss;
//ss << sockid;
//__android_log_write(ANDROID_LOG_ERROR, "Tikrinu1",ss.str().c_str());
//__android_log_write(ANDROID_LOG_INFO, "Tikrinu2","BANDAUUU");
//__android_log_print(ANDROID_LOG_INFO, "Native", "TEST LOG");
		addr.sin_family = AF_INET;
		bcopy ( hostEntry->h_addr, &(addr.sin_addr.s_addr), hostEntry->h_length);
		addr.sin_port = htons((u_short)port);
		if(mode ==2)setsync(1);
/***********FIX**************/
		int SOCKET_TIMEOUT=5;
		struct timeval tv;
		tv.tv_sec = SOCKET_TIMEOUT;
		tv.tv_usec = 0 ;
		setsockopt (sockid, SOL_SOCKET, SO_SNDTIMEO, (void*)&tv, sizeof tv);
		setsockopt (sockid, SOL_SOCKET, SO_RCVTIMEO, (void*)&tv, sizeof tv);
/********ENDFIX**************/
		if(connect(sockid, (struct sockaddr*)&addr, sizeof(sockaddr)) == SOCKET_ERROR)
		{
			closesocket(sockid);
			return false;
		}
		if (mode == 1)setsync(1);
		return true;
	}
#endif

//good
#if defined( __WIN32__ ) || defined( WIN32 ) || defined( _WIN32 )
bool CSocket::tcplisten(int port, int max, int mode)
{
	if ((sockid = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == INVALID_SOCKET) return false;
	SOCKADDR_IN addr;
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = INADDR_ANY;
	addr.sin_port = htons(port);
	if (mode)setsync(1);
	if (bind(sockid, reinterpret_cast<LPSOCKADDR>(&addr), sizeof(SOCKADDR_IN)) == SOCKET_ERROR)
	{
		closesocket(sockid);
		return false;
	}
	if (listen(sockid, max) == SOCKET_ERROR)
	{
		closesocket(sockid);
		return false;
	}
	return true;
}
#else
	bool CSocket::tcplisten(int port, int max, int mode)
	{
		if ((sockid = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == INVALID_SOCKET) return false;
		sockaddr_in addr; 
		addr.sin_family = AF_INET;
		addr.sin_addr.s_addr = INADDR_ANY;
		addr.sin_port = htons(port);
		if (mode)setsync(1);
		if (bind(sockid, (struct sockaddr*)&addr, sizeof(sockaddr)) == SOCKET_ERROR)
		{
			closesocket(sockid);
			return false;
		}
		if (listen(sockid, max) == SOCKET_ERROR)
		{
			closesocket(sockid);
			return false;
		}
		return true;
	}
#endif

//good
#if defined( __WIN32__ ) || defined( WIN32 ) || defined( _WIN32 )
CSocket::CSocket(SOCKET sock)
{
	sockid = sock;
	udp = false;
	format = 0;
}
#else
	CSocket::CSocket(int sock)
	{
		sockid = sock;
		udp = false;
		format = 0;
	}
#endif

//good
CSocket::CSocket() //:sockid(0)
{
	udp = false;
	format = 0;
}


//good
CSocket::~CSocket()
{
	//if (sockid < 0)return;
	shutdown(sockid, 1);
	closesocket(sockid);
}

//good
#if defined( __WIN32__ ) || defined( WIN32 ) || defined( _WIN32 )
CSocket* CSocket::tcpaccept(int mode) const
{
	//if (sockid < 0)return nullptr;
	SOCKET sock2;
	if ((sock2 = accept(sockid, reinterpret_cast<SOCKADDR *>(&SenderAddr), &SenderAddrSize)) != INVALID_SOCKET)
	{
		auto sockit = new CSocket(sock2);
		if (mode >= 1)sockit->setsync(1);
		return sockit;
	}
	return nullptr;
}
#else
	CSocket* CSocket::tcpaccept(int mode) const
	{
		if (sockid<0)return NULL;
		int sock2;
		if ((sock2 = accept(sockid, (sockaddr *)&SenderAddr, (socklen_t*)&SenderAddrSize)) != INVALID_SOCKET)
		{
			CSocket*sockit = new CSocket(sock2);
			if (mode >= 1)sockit->setsync(1);
			return sockit;
		}
		return NULL;
	}
#endif

//good
#if defined( __WIN32__ ) || defined( WIN32 ) || defined( _WIN32 )
char* CSocket::tcpip() const
{
	//if (sockid < 0)return NULL;
	if (getpeername(sockid, reinterpret_cast<SOCKADDR *>(&SenderAddr), &SenderAddrSize) == SOCKET_ERROR)return nullptr;
	return inet_ntoa(SenderAddr.sin_addr);
}
#else
	char* CSocket::tcpip() const
	{
		if(sockid<0)return NULL;
		if(getpeername(sockid, (sockaddr *)&SenderAddr, (socklen_t*)&SenderAddrSize) == SOCKET_ERROR)return NULL;
		return inet_ntoa(SenderAddr.sin_addr);
	}
#endif

//good
void CSocket::setnagle(bool enabled) const
{
	if (sockid < 0)return;
	setsockopt(sockid, IPPROTO_TCP, TCP_NODELAY, reinterpret_cast<char*>(&enabled), sizeof(bool));
}

//good
#if defined( __WIN32__ ) || defined( WIN32 ) || defined( _WIN32 )
bool CSocket::tcpconnected() const
{
	if (sockid < 0)return false;
	char b;
	if (recv(sockid, &b, 1, MSG_PEEK) == SOCKET_ERROR)
		if (WSAGetLastError() != WSAEWOULDBLOCK)return false;
	return true;
}
#else
	bool CSocket::tcpconnected() const
	{
		if (sockid<0)return false;
		char b;
		if (recv(sockid, &b, 1, MSG_PEEK) == SOCKET_ERROR)
			return false;
		return true;
	}
#endif

//good
#if defined( __WIN32__ ) || defined( WIN32 ) || defined( _WIN32 )
int CSocket::setsync(int mode) const
{
	if (sockid < 0)return -1;
	u_long i = mode;
	return ioctlsocket(sockid, FIONBIO, &i); //ioctl
}
#else
	int CSocket::setsync(int mode) const
	{
		if (sockid < 0)return -1;
		u_long i = mode;
		return ioctl(sockid, FIONBIO, &i);
	}
#endif

//good
#if defined( __WIN32__ ) || defined( WIN32 ) || defined( _WIN32 )
bool CSocket::udpconnect(int port, int mode)
{
	SOCKADDR_IN addr;
	if ((sockid = socket(AF_INET, SOCK_DGRAM, 0)) == SOCKET_ERROR)
		return false;
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = INADDR_ANY;
	addr.sin_port = htons(port);
	if (mode)setsync(1);
	if (bind(sockid, reinterpret_cast<SOCKADDR *>(&addr), sizeof(SOCKADDR_IN)) == SOCKET_ERROR)
	{
		closesocket(sockid);
		return false;
	}
	udp = true;
	return true;
}
#else
	bool CSocket::udpconnect(int port, int mode)
	{
		sockaddr_in addr; 
		if ((sockid = socket(AF_INET, SOCK_DGRAM, 0)) == SOCKET_ERROR)
			return false;
		addr.sin_family = AF_INET;
		addr.sin_addr.s_addr = INADDR_ANY;
		addr.sin_port = htons(port);
		if (mode)setsync(1);
		if (bind(sockid, (sockaddr *)&addr, sizeof(sockaddr_in)) == SOCKET_ERROR) 
		{
			closesocket(sockid);
			return false;
		}
		udp = true;
		return true;
	}
#endif

//good
#if defined( __WIN32__ ) || defined( WIN32 ) || defined( _WIN32 )
int CSocket::sendmessage(char* ip, int port, CBuffer* source)
{
	//if (sockid < 0)return -1;
	auto size = 0;
	SOCKADDR_IN addr;
	if (udp)
	{
		size = std::min(source->count, 8195);
		addr.sin_family = AF_INET;
		addr.sin_port = htons(port);
		addr.sin_addr.s_addr = inet_addr(ip);
		size = sendto(sockid, source->data, size, 0, reinterpret_cast<SOCKADDR *>(&addr), sizeof(SOCKADDR_IN));
	}
	else
	{
		CBuffer sendbuff;
		sendbuff.clear();
		if (format == 0)
		{
			sendbuff.writeushort(source->count);
			sendbuff.addBuffer(source);
			size = send(sockid, sendbuff.data, sendbuff.count, 0);
		}
		else if (format == 1)
		{
			sendbuff.addBuffer(source);
			sendbuff.writechars(formatstr);
			size = send(sockid, sendbuff.data, sendbuff.count, 0);
		}
		else if (format == 2)
			size = send(sockid, source->data, source->count, 0);
	}

	if (size == SOCKET_ERROR)return -WSAGetLastError();

	return size;
}
#else
	int CSocket::sendmessage(char *ip, int port, CBuffer *source)
	{

		if (sockid<0)return -1;
		int size = 0;
		sockaddr_in addr; 
		if (udp)
		{
			size = std::min(source->count, 8195);
			addr.sin_family = AF_INET;
			addr.sin_port = htons(port);
			addr.sin_addr.s_addr = inet_addr(ip);
			size = sendto(sockid, source->data, size, 0, (sockaddr *)&addr, sizeof(sockaddr_in));
		}
		else
		{
			CBuffer sendbuff;
			sendbuff.clear();
			if (format == 0)
			{
				sendbuff.writeushort(source->count);
				sendbuff.addBuffer(source);
				size = send(sockid, sendbuff.data, sendbuff.count, 0);
			}
			else if (format == 1)
			{
				sendbuff.addBuffer(source);
				sendbuff.writechars(formatstr);
				size = send(sockid, sendbuff.data, sendbuff.count, 0);
			}
			else if (format == 2)
				size = send(sockid, source->data, source->count, 0);
		}

		if (size == SOCKET_ERROR)return SOCKET_ERROR;

		return size;
	}

#if (CC_TARGET_PLATFORM != CC_PLATFORM_IOS && CC_TARGET_PLATFORM != CC_PLATFORM_WIN32)
int CSocket::sendmessageRohc(char *ip, int port, CBuffer *source)
{
    if (sockid<0)return -1;
    int size = 0;
    sockaddr_in addr;
    if (udp)
    {
        size = std::min(source->count, 8195);
        addr.sin_family = AF_INET;
        addr.sin_port = htons(port);
        addr.sin_addr.s_addr = inet_addr(ip);
        size = sendto(sockid, source->data, size, 0, (sockaddr *)&addr, sizeof(sockaddr_in));
    }
    else
    {
        CBuffer sendbuff;
        sendbuff.clear();
        if (format == 0)
        {
            sendbuff.writeushort(source->count);
            sendbuff.addBuffer(source);
            size = send(sockid, sendbuff.data, sendbuff.count, 0);
            
//            int BUFFER_SIZE = 2048;
//            uint8_t rohc_buffer[BUFFER_SIZE];
//            rohc_buf compressed_rohc_packet = rohc_buf_init_empty(rohc_buffer, BUFFER_SIZE);
//            RohcCompression::compress(&sendbuff.data, &compressed_rohc_packet);
//            
//            char *m = (char *)malloc(compressed_rohc_packet.len);
//            for(auto i = 0; i < compressed_rohc_packet.len; i++)
//            {
//                m[i] = rohc_buf_byte_at(compressed_rohc_packet,i);
//            }
//            
//            size = sendto(sockid, m, compressed_rohc_packet.len, 0,
//                          (sockaddr *)&addr, sizeof(sockaddr_in));
        }
        else if (format == 1)
        {
            sendbuff.addBuffer(source);
            sendbuff.writechars(formatstr);
            size = send(sockid, sendbuff.data, sendbuff.count, 0);
        }
        else if (format == 2)
            size = send(sockid, source->data, source->count, 0);
    }
    
    if (size == SOCKET_ERROR)return SOCKET_ERROR;
    
    return size;
}
#endif
#endif

//good
int CSocket::receivetext(char* buf, int max)
{
	auto len = static_cast<int>(strlen(formatstr));
	if ((max = (int)recv(sockid, buf, max, MSG_PEEK)) != SOCKET_ERROR)
	{
		int i, ii;
		for (i = 0; i < max; i++)
			{
				for (ii = 0; ii < len; ii++)
					if (buf[i + ii] != formatstr[ii])
						break;
				if (ii == len)
					return (int)recv(sockid, buf, i + len, 0);
			}
	}
	return -1;
}

//good
#if defined( __WIN32__ ) || defined( WIN32 ) || defined( _WIN32 )
int CSocket::receivemessage(int len, CBuffer* destination)
{
	//if (sockid < 0)return -1;
	auto size = -1;
	char* buff = nullptr;
	if (udp)
	{
		size = 8195;
		buff = new char[size];
		size = recvfrom(sockid, buff, size, 0, reinterpret_cast<SOCKADDR *>(&SenderAddr), &SenderAddrSize);
	}
	else
	{
		if (format == 0 && !len)
		{
			unsigned short length;
			if (recv(sockid, reinterpret_cast<char*>(&length), 2, 0) == SOCKET_ERROR)return -1;
			buff = new char[length];
			size = recv(sockid, buff, length, 0);
		}
		else if (format == 1 && !len)
		{
			size = 65536;
			buff = new char[size];
			size = receivetext(buff, size);
		}
		else if (format == 2 || len > 0)
		{
			buff = new char[len];
			size = recv(sockid, buff, len, 0);
		}
	}
	if (size > 0)
	{
		destination->clear();
		destination->addBuffer(buff, size);
	}
	if (buff != nullptr){
		//delete buff;
		buff = nullptr;
	}
	return size;
}
#else
int CSocket::receivemessage(int len, CBuffer*destination)
{
//__android_log_print(ANDROID_LOG_INFO, "debug info", "viduj len: %d", len);
	if(sockid<0)return -1;
	int size = -1;
	char* buff = NULL;
	if(udp)
	{
		size = 8195;
		buff = new char[size];
		size = recvfrom(sockid, buff, size, 0, (sockaddr *)&SenderAddr, (socklen_t*)&SenderAddrSize);
	} else
	{
//__android_log_print(ANDROID_LOG_INFO, "debug info", "else");
		if(format == 0 && !len)
		{
			unsigned short length;
			if(recv(sockid, (char*)&length, 2, 0) == SOCKET_ERROR)return -1;
			buff = new char[length];
			size = recv(sockid, buff, length, 0);
		} else if(format == 1 && !len)
		{
			size = 65536;
			buff = new char[size];
			size = receivetext(buff, size);
		} else if(format == 2 || len > 0)
		{
//__android_log_print(ANDROID_LOG_INFO, "debug info", "trecias");
			buff = new char[len];
			size = recv(sockid, buff, len, 0);
		}
	}
	if(size > 0)
	{
		destination->clear();
		destination->addBuffer(buff, size);
	}
	if(buff != NULL)delete buff;
//__android_log_print(ANDROID_LOG_INFO, "debug info", "return size: %d",size);
	return size;
}
#endif

//good
#if defined( __WIN32__ ) || defined( WIN32 ) || defined( _WIN32 )
int CSocket::peekmessage(int size, CBuffer* destination) const
{
	//if (sockid < 0)return -1;
	if (size == 0)size = 65536;
	auto buff = new char[size];
	size = recvfrom(sockid, buff, size, MSG_PEEK, reinterpret_cast<SOCKADDR *>(&SenderAddr), &SenderAddrSize);

	if (size < 0)
	{
		//delete buff;
		buff = nullptr;
		return -1;
	}
	destination->clear();
	destination->addBuffer(buff, size);
	//delete buff;
	buff = nullptr;
	return size;
}
#else
int CSocket::peekmessage(int size, CBuffer*destination) const
{
	if (sockid<0)return -1;
	if (size == 0)size = 65536;
	char *buff = new char[size];
	size = recvfrom(sockid, buff, size, MSG_PEEK, (sockaddr *)&SenderAddr, (socklen_t*)&SenderAddrSize);

	if (size < 0)
	{
		delete[] buff;
		return -1;
	}
	destination->clear();
	destination->addBuffer(buff, size);
	delete[] buff;
	return size;
}
#endif

//good
#if defined( __WIN32__ ) || defined( WIN32 ) || defined( _WIN32 )
int CSocket::lasterror()
{
	return WSAGetLastError();
}
#else
	int CSocket::lasterror()
	{
		return SOCKET_ERROR;
	}
#endif

//good
#if defined( __WIN32__ ) || defined( WIN32 ) || defined( _WIN32 )
char* CSocket::GetIp(char* address)
{
	SOCKADDR_IN addr;
	LPHOSTENT hostEntry;
	if ((hostEntry = gethostbyname(address)) == nullptr) return nullptr;
	addr.sin_addr = *reinterpret_cast<LPIN_ADDR>(*hostEntry->h_addr_list);
	return inet_ntoa(addr.sin_addr);
}
#else
	char* CSocket::GetIp(char*address)
	{
		sockaddr_in addr; 
		hostent* hostEntry; 
		if ((hostEntry = gethostbyname(address)) == NULL) return NULL;
		addr.sin_addr.s_addr = htonl(INADDR_ANY);
		return inet_ntoa(addr.sin_addr);
	}
#endif

//good
char* CSocket::lastinIP(void)
{
	return inet_ntoa(SenderAddr.sin_addr);
}

//good
unsigned short CSocket::lastinPort(void)
{
	return ntohs(SenderAddr.sin_port);
}

//good
int CSocket::SetFormat(int mode, char* sep)
{
	int previous = format;
	format = mode;
	if (mode == 1 && strlen(sep) > 0)
		strcpy(formatstr, sep);
	return previous;
}

//good
#if defined( __WIN32__ ) || defined( WIN32 ) || defined( _WIN32 )
int CSocket::SockExit(void)
{
	WSACleanup();
	return 1;
}

int CSocket::SockStart(void)
{
	WSADATA wsaData;
	WSAStartup(MAKEWORD(1, 1), &wsaData);
	return 1;
}
#else
	int CSocket::SockExit(void)
	{
		if(sockid<0)return 0;
		shutdown(sockid, 1);
		closesocket(sockid);  //TODO: This wont work! sockid is not static and this is a static method. Need to devise
//CSocket::~CSocket();
		sockid=0;//a means of closing out the socket system.
		return 1;
	}
	int CSocket::SockStart(void)
	{
//WSADATA wsaData;
//WSAStartup(MAKEWORD(1,1),&wsaData);
		return 1;
	}
#endif

//good
char* CSocket::myhost()
{
	static char buf[16];
	gethostname(buf, 16);
	return buf;
}
