#pragma once
#include "common.h"
#include "buffer.h"

class CSocket
{
	bool udp;
	int format;
	char formatstr[30];
#if defined( __WIN32__ ) || defined( WIN32 ) || defined( _WIN32 )
	static SOCKADDR_IN SenderAddr; 
#else
#define closesocket(s) close(s);
	static sockaddr_in SenderAddr; 
#endif
	int receivetext(char*buf, int max);
public:

	
	#if defined( __WIN32__ ) || defined( WIN32 ) || defined( _WIN32 )
		SOCKET sockid; //static int sockid;
	explicit CSocket(SOCKET sock); //CSocket(int sock);
	#else
	#define closesocket(s) close(s);
		static int sockid;
		CSocket(int sock);
	#endif

	CSocket();
	~CSocket();
	bool tcpconnect(char*address, int port, int mode);
	bool tcplisten(int port, int max, int mode);
	CSocket* tcpaccept(int mode) const;
	char* tcpip() const;
	void setnagle(bool enabled) const;
	bool tcpconnected() const;
	int setsync(int mode) const;
	bool udpconnect(int port, int mode);
	int sendmessage(char*ip, int port, CBuffer* source);
	int receivemessage(int len, CBuffer*destination);
	int peekmessage(int size, CBuffer*destination) const;
	static int lasterror();
	static char* GetIp(char*address);
	static int SockExit(void);
	static int SockStart(void);
	static char* lastinIP(void);
	static unsigned short lastinPort(void);
	static char* myhost();
	int SetFormat(int mode, char* sep);
};
