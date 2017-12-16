//
//  rawtcp.h
//  GameTrafficOptimization
//
//  Created by Dainius Kreivys on 2017-01-30.
//
//

#ifndef rawtcp_h
#define rawtcp_h

/*
 Raw TCP packets
 Silver Moon (m00n.silv3r@gmail.com)
 */
#include <stdio.h> //for printf
#include <string.h> //memset
#include <sys/socket.h>    //for socket ofcourse
#include <stdlib.h> //for exit(0);
#include <errno.h> //For errno - the error number
#include <netinet/tcp.h>   //Provides declarations for tcp header
#include <netinet/ip.h>    //Provides declarations for ip header
#include <netinet/in.h>
#include <arpa/inet.h>
/*
 96 bit (12 bytes) pseudo header needed for tcp header checksum calculation
 */
struct pseudo_header
{
    u_int32_t source_address;
    u_int32_t dest_address;
    u_int8_t placeholder;
    u_int8_t protocol;
    u_int16_t tcp_length;
};




class RawTcp
{
public:
    static char* createRawTcpPacket(int &packetLength);
private:
    static unsigned short csum(unsigned short *ptr,int nbytes);
};

#endif /* rawtcp_h */
