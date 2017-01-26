//
//  SctpSocket.hpp
//  GameTrafficOptimization
//
//  Created by Dainius Kreivys on 2017-01-26.
//
//

#ifndef SctpSocket_hpp
#define SctpSocket_hpp

//#if (CC_TARGET_PLATFORM != CC_PLATFORM_IOS)

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
#include "transport_protocols/tcp/common.h"
#include "transport_protocols/tcp/buffer.h"
#else
#include "common.h"
#include "buffer.h"
#endif
#include <usrsctp.h>

struct socket *sock;
struct sockaddr *addr, *addrs;
struct sockaddr_in addr4;
struct sockaddr_in6 addr6;
struct sctp_udpencaps encaps;
//struct sctpstat stat;
char buffer[80];
int i, n;
int done = 0;

class SctpSocket
{
public:
	
};

//#endif

#endif
