//
//  RohcCompression.cpp
//  GameTrafficOptimization
//
//  Created by Dainius Kreivys on 2017-01-15.
//
//
#if (CC_TARGET_PLATFORM != CC_PLATFORM_IOS && CC_TARGET_PLATFORM != CC_PLATFORM_WIN32)
#include "RohcCompression.hpp"
//#include <stdio.h>  /* for the printf() function */
//#include <iostream> /* for std::* functions */
#include <string.h>      /* for the strlen() */
#include <time.h>		/* required by time() */
#include <rohc/rohc.h> /* includes required to use the compression part of the ROHC library */
#include <rohc/rohc_buf.h>  /* for the rohc_buf_*() functions */
#include <rohc/rohc_comp.h>   /* for rohc_comp_*() functions */
#include <rohc/rohc_decomp.h> /* for rohc_decomp_*() functions */
//struct rohc_comp;
//struct rohc_decomp;

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
#include "transport_protocols/tcp/Common.h"
#include "utils/ScreenLog.hpp"
//#include "transport_protocols/tcp/socket"
#else
#include "Common.h"
#include "ScreenLog.hpp"
//#include "socket.h"
#endif

#include <stdio.h> //for printf
#include <string.h> //memset
#include <stdlib.h>
#include <cstring>
//#define printf(MESSAGE,args...) g_screenLog->log( LL_DEBUG, MESSAGE, args);
//#define printf(MESSAGE) g_screenLog->log( LL_DEBUG, MESSAGE);
//#define printf(MESSAGE,args...) { \
//const char *A[] = {MESSAGE}; \
//if(sizeof(A) > 0) {\
//g_screenLog->log( LL_DEBUG, *A, ##args); \
//} else {\
//g_screenLog->log( LL_DEBUG, "\n"); \
//}\
//}

#include "rawtcp.h"



/* The size (in bytes) of the buffers used in the program */
#define BUFFER_SIZE 2048

/* The payload for the fake IP packet */
//#define FAKE_PAYLOAD "hello, ROHC world!"
#define FAKE_PAYLOAD "ABCDEFGHIJKLMNOPQRSTUVWXYZ123456789012345678901234"

#define PACKET_COUNT 10

struct iphdr
{
#if __BYTE_ORDER == __LITTLE_ENDIAN
    unsigned int ihl:4;
    unsigned int version:4;
#elif __BYTE_ORDER == __BIG_ENDIAN
    unsigned int version:4;
    unsigned int ihl:4;
#else
# error  "Please fix <bits/endian.h>"
#endif
    
    uint8_t tos;
    uint16_t tot_len;
    uint16_t id;
    uint16_t frag_off;
    uint8_t ttl;
    uint8_t protocol;
    uint16_t check;
    uint32_t saddr;
    uint32_t daddr;
    /*The options start here. */
};

/* return a random number every time it is called */
static int gen_random_num(const struct rohc_comp*const comp,
						  void*const user_context)
{
	return rand();
}
/**
 * @brief Dump the given network packet on standard output
 *
 * @param packet  The packet to dump
 */
static void dump_packet(const struct rohc_buf packet)
{
	size_t i;
	
	for(i = 0; i < packet.len; i++)
	{
		printf("0x%02x ", rohc_buf_byte_at(packet, i));
		if(i != 0 && ((i + 1) % 8) == 0)
		{
			printf("\n");
		}
	}
	if(i != 0 && ((i + 1) % 8) != 0) /* be sure to go to the line */
	{
		printf("\n");
	}
}
/*
 The IP Header Checksum is computed on the header fields only.
 Before starting the calculation, the checksum fields (octets 11 and 12)
 are made equal to zero.
 
 In the example code,
 u16 buff[] is an array containing all octets in the header with octets 11 and 12 equal to zero.
 u16 len_ip_header is the length (number of octets) of the header.
 **************************************************************************
 Function: ip_sum_calc
 Description: Calculate the 16 bit IP sum.
 ***************************************************************************
 DK. Don't know if this wokrs...
 */
uint16_t ip_sum_calc(uint16_t len_ip_header, uint8_t buff[])
{
	uint16_t word16;
	uint32_t sum=0;
	uint16_t i;
	
	// make 16 bit words out of every two adjacent 8 bit words in the packet
	// and add them up
	for (i=0;i<len_ip_header;i=i+2){
		word16 =((buff[i]<<8)&0xFF00)+(buff[i+1]&0xFF);
		sum = sum + (uint32_t) word16;
	}
	
	// take only 16 bits out of the 32 bit sum and add up the carries
	while (sum>>16)
		sum = (sum & 0xFFFF)+(sum >> 16);
	
	// one's complement the result
	sum = ~sum;
	
	return ((uint16_t) sum);
}

//forwarded the declarations of struct pointers
rohc_comp* RohcCompression::_compressor;
rohc_decomp* RohcCompression::_decompressor;

// Define some constants.
#define IP4_HDRLEN 20         // IPv4 header length
#define TCP_HDRLEN 20         // TCP header length, excludes options data

#define __FAVOR_BSD           // Use BSD format of tcp header

// Function prototypes
uint16_t checksum (uint16_t *, int);
uint16_t tcp4_checksum (struct ip iphdr, struct tcphdr tcphdr);//(struct ip, struct tcphdr, uint8_t *, int);
char *allocate_strmem (int);
uint8_t *allocate_ustrmem (int);
int *allocate_intmem (int);

void RohcCompression::performTest()
{
	printf("ROHC version %s\n", rohc_version());
	//srand48(time(nullptr)); /* initialize the random generator with the current time */ //-don't know if it is needed. Why compressor needs random value?
	if(!RohcCompression::init()) return;
	
	uint8_t ip_buffer[BUFFER_SIZE];
	rohc_buf ip_packet = rohc_buf_init_empty(ip_buffer, BUFFER_SIZE);
    
    //!TEST 1:
    //int packetLength;
    //auto buffer = RawTcp::createRawTcpPacket(packetLength);
    //rohc_buf_append(&ip_packet, (uint8_t *)buffer, packetLength);
    //!TEST 2:
    //Datagram to represent the packet
//    char datagram[4096] , source_ip[32] , *data , *pseudogram;
//    
//    //zero out the packet buffer
//    memset (datagram, 0, 4096);
//    
//    //IP header
//    struct iphdr *iph = (struct iphdr *) datagram;
//    
//    //TCP header
//    struct tcphdr *tcph = (struct tcphdr *) (datagram + sizeof (struct ip));
//    struct sockaddr_in sin;
//    struct pseudo_header psh;
//    
//    //Data part
//    data = datagram + sizeof(struct iphdr) + sizeof(struct tcphdr);
//    strcpy(data , "ABCDEFGHIJKLMNOPQRSTUVWXYZ");
//    
//    //Fill in the IP Header
//    iph->ihl = 5;
//    iph->version = 4;
//    iph->tos = 0;
//    iph->tot_len = sizeof (struct iphdr) + sizeof (struct tcphdr) + strlen(data);
//    iph->id = 0;/* ID is not important for the example */ //htonl (54321); //Id of this packet
//    iph->frag_off = 0;
//    iph->ttl = 255;
//    iph->protocol = IPPROTO_TCP;
//    iph->check = 0;      //Set to 0 before calculating checksum
//    iph->saddr = inet_addr ( source_ip );    //Spoof the source ip address
//    iph->daddr = sin.sin_addr.s_addr;
//    iph->saddr = htonl(0x01020304); /* source address 1.2.3.4 */
//    iph->daddr = htonl(0x05060708); /* destination addr. 5.6.7.8 */
//    
//    //Ip checksum
//    iph->check = csum ((unsigned short *) datagram, iph->tot_len);
//    
//    //TCP Header
//    tcph->th_sport = htons (1234); //source
//    tcph->th_dport = htons (80); //dest
//    tcph->th_seq = 0; //seq
//    tcph->th_ack = 0; //ack_seq
//    tcph->th_off = 5;  //tcp header size //doff
//    tcph->th_flags = TH_SYN;
//    tcph->th_win = htons (5840); /* maximum allowed window size */
//    tcph->th_sum = 0; //leave checksum 0 now, filled later by pseudo header
//    tcph->th_urp = 0; //urg_ptr
//    
//    //Now the TCP checksum
//    psh.source_address = inet_addr( source_ip );
//    psh.dest_address = sin.sin_addr.s_addr;
//    psh.placeholder = 0;
//    psh.protocol = IPPROTO_TCP;
//    psh.tcp_length = htons(sizeof(struct tcphdr) + strlen(data) );
//    
//    int psize = sizeof(struct pseudo_header) + sizeof(struct tcphdr) + strlen(data);
//    pseudogram = (char *)malloc(psize);
//    
//    memcpy(pseudogram , (char*) &psh , sizeof (struct pseudo_header));
//    memcpy(pseudogram + sizeof(struct pseudo_header) , tcph , sizeof(struct tcphdr) + strlen(data));
//    
//    tcph->th_sum = csum( (unsigned short*) pseudogram , psize);
//    
//    rohc_buf_append(&ip_packet, (uint8_t *)datagram, iph->tot_len);
    //!Test 3: WORKS! TCP IPv4 WITHOUT OPTIONS AND PAYLOAD
    int i, status,  *ip_flags, *tcp_flags; //sd,
    //const int on = 1;
    char *interface, *target, *src_ip, *dst_ip;
    struct ip iphdr;
    struct tcphdr tcphdr;
    uint8_t *packet;
    struct addrinfo hints, *res;
    struct sockaddr_in *ipv4;//, sin;
    //struct ifreq ifr;
    void *tmp;
    
    // Allocate memory for various arrays.
    packet = allocate_ustrmem (IP_MAXPACKET);
    interface = allocate_strmem (40);
    target = allocate_strmem (40);
    src_ip = allocate_strmem (INET_ADDRSTRLEN);
    dst_ip = allocate_strmem (INET_ADDRSTRLEN);
    ip_flags = allocate_intmem (4);
    tcp_flags = allocate_intmem (8);
    
    
    // Source IPv4 address: you need to fill this out
    strcpy (src_ip, "192.168.1.132");
    
    // Destination URL or IPv4 address: you need to fill this out
    strcpy (target, "www.google.com");
    
    // Fill out hints for getaddrinfo().
    memset (&hints, 0, sizeof (struct addrinfo));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = hints.ai_flags | AI_CANONNAME;
    
    // Resolve target using getaddrinfo().
    if ((status = getaddrinfo (target, NULL, &hints, &res)) != 0) {
        fprintf (stderr, "getaddrinfo() failed: %s\n", gai_strerror (status));
        exit (EXIT_FAILURE);
    }
    ipv4 = (struct sockaddr_in *) res->ai_addr;
    tmp = &(ipv4->sin_addr);
    if (inet_ntop (AF_INET, tmp, dst_ip, INET_ADDRSTRLEN) == NULL) {
        status = errno;
        fprintf (stderr, "inet_ntop() failed.\nError message: %s", strerror (status));
        exit (EXIT_FAILURE);
    }
    freeaddrinfo (res);
    
    // IPv4 header
    
    // IPv4 header length (4 bits): Number of 32-bit words in header = 5
    iphdr.ip_hl = IP4_HDRLEN / sizeof (uint32_t);
    
    // Internet Protocol version (4 bits): IPv4
    iphdr.ip_v = 4;
    
    // Type of service (8 bits)
    iphdr.ip_tos = 0;
    
    // Total length of datagram (16 bits): IP header + TCP header
    iphdr.ip_len = htons (IP4_HDRLEN + TCP_HDRLEN + strlen(FAKE_PAYLOAD));
    
    // ID sequence number (16 bits): unused, since single datagram
    iphdr.ip_id = htons (0);
    
    // Flags, and Fragmentation offset (3, 13 bits): 0 since single datagram
    
    // Zero (1 bit)
    ip_flags[0] = 0;
    
    // Do not fragment flag (1 bit)
    ip_flags[1] = 0;
    
    // More fragments following flag (1 bit)
    ip_flags[2] = 0;
    
    // Fragmentation offset (13 bits)
    ip_flags[3] = 0;
    
    iphdr.ip_off = htons ((ip_flags[0] << 15)
                          + (ip_flags[1] << 14)
                          + (ip_flags[2] << 13)
                          +  ip_flags[3]);
    
    // Time-to-Live (8 bits): default to maximum value
    iphdr.ip_ttl = 255;
    
    // Transport layer protocol (8 bits): 6 for TCP
    iphdr.ip_p = IPPROTO_TCP;
    
    // Source IPv4 address (32 bits)
    if ((status = inet_pton (AF_INET, src_ip, &(iphdr.ip_src))) != 1) {
        fprintf (stderr, "inet_pton() failed.\nError message: %s", strerror (status));
        exit (EXIT_FAILURE);
    }
    
    // Destination IPv4 address (32 bits)
    if ((status = inet_pton (AF_INET, dst_ip, &(iphdr.ip_dst))) != 1) {
        fprintf (stderr, "inet_pton() failed.\nError message: %s", strerror (status));
        exit (EXIT_FAILURE);
    }
    
    // IPv4 header checksum (16 bits): set to 0 when calculating checksum
    iphdr.ip_sum = 0;
    iphdr.ip_sum = checksum ((uint16_t *) &iphdr, IP4_HDRLEN);
    
    // TCP header
    
    // Source port number (16 bits)
    tcphdr.th_sport = htons (60);
    
    // Destination port number (16 bits)
    tcphdr.th_dport = htons (80);
    
    // Sequence number (32 bits)
    tcphdr.th_seq = htonl (0);
    
    // Acknowledgement number (32 bits): 0 in first packet of SYN/ACK process
    tcphdr.th_ack = htonl (0);
    
    // Reserved (4 bits): should be 0
    tcphdr.th_x2 = 0;
    
    // Data offset (4 bits): size of TCP header in 32-bit words
    tcphdr.th_off = TCP_HDRLEN / 4;
    
    // Flags (8 bits)
    
    // FIN flag (1 bit)
    tcp_flags[0] = 0;
    
    // SYN flag (1 bit): set to 1
    tcp_flags[1] = 1;
    
    // RST flag (1 bit)
    tcp_flags[2] = 0;
    
    // PSH flag (1 bit)
    tcp_flags[3] = 0;
    
    // ACK flag (1 bit)
    tcp_flags[4] = 0;
    
    // URG flag (1 bit)
    tcp_flags[5] = 0;
    
    // ECE flag (1 bit)
    tcp_flags[6] = 0;
    
    // CWR flag (1 bit)
    tcp_flags[7] = 0;
    
    tcphdr.th_flags = 0;
    for (i=0; i<8; i++) {
        tcphdr.th_flags += (tcp_flags[i] << i);
    }
    
    // Window size (16 bits)
    tcphdr.th_win = htons (65535);
    
    // Urgent pointer (16 bits): 0 (only valid if URG flag is set)
    tcphdr.th_urp = htons (0);
    
    // TCP checksum (16 bits)
    tcphdr.th_sum = tcp4_checksum (iphdr, tcphdr);
    
    // Prepare packet.
    
    // First part is an IPv4 header.
    memcpy (packet, &iphdr, IP4_HDRLEN * sizeof (uint8_t));
    
    // Next part of packet is upper layer protocol header.
    memcpy ((packet + IP4_HDRLEN), &tcphdr, TCP_HDRLEN * sizeof (uint8_t));
    

    rohc_buf_append(&ip_packet, (uint8_t *)packet, IP4_HDRLEN + TCP_HDRLEN);
    
    printf("Payload length: %d \n", strlen(FAKE_PAYLOAD));
    rohc_buf_append(&ip_packet, (uint8_t *)FAKE_PAYLOAD, strlen(FAKE_PAYLOAD));
    
    //!Test  4: unsuported bersion of header
    //createRohcPacketTcpIp((char*)FAKE_PAYLOAD, &ip_packet);
    
    //!Test 5: just ip, works
	//createIpPacket((char*)FAKE_PAYLOAD, &ip_packet);
	
	printf("Uncompressed Packet Length:\t%zu\n", ip_packet.len);
	
	for(auto i = 0; i < PACKET_COUNT; i++)
	{
		uint8_t rohc_buffer[BUFFER_SIZE];
		rohc_buf compressed_rohc_packet = rohc_buf_init_empty(rohc_buffer, BUFFER_SIZE);  /* the packet that will contain the resulting ROHC packet */
//		char *m = (char *)malloc(compressed_rohc_packet.len);
//		for(auto j = 0; j < compressed_rohc_packet.len; j++)
//		{
//			m[j] = rohc_buf_byte_at(compressed_rohc_packet, j);
//		}
		
		if(!compress(&ip_packet, &compressed_rohc_packet)) return;
		printf("Packet no: %d compressed Packet Length:\t%zu\n", i, compressed_rohc_packet.len);
		
		//printLastCompressedPacketInfo();
		
		rohc_buf decompressed_ip_packet;
		if(!decompress(&compressed_rohc_packet, decompressed_ip_packet)) return;
		printf("Packet no: %d decompressed Packet Length:\t%zu\n", i, decompressed_ip_packet.len);
	}
	
    // Close socket descriptor.
    //close (sd);
    
    // Free allocated memory.
    ::free (packet);
    ::free (interface);
    ::free (target);
    ::free (src_ip);
    ::free (dst_ip);
    ::free (ip_flags);
    ::free (tcp_flags);
	RohcCompression::free();
}
void RohcCompression::printLastCompressedPacketInfo()
{
	rohc_comp_last_packet_info2_t info;
	info.version_major=0;
	info.version_minor=0;
	if(!rohc_comp_get_last_packet_info2(_compressor, &info)) return;
	printf( "context_id = %d\n", info.context_id);
	printf( "is_context_init = %d\n", info.is_context_init);
	printf( "context_mode = %d\n", info.context_mode);
	printf( "context_state = %d\n", info.context_state);
	printf( "context_used = %d\n", info.context_used);
	printf( "profile_id = %d\n",info.profile_id);
	printf( "packet_type = %d\n", info.packet_type);
	printf( "total_last_uncomp_size = %lu\n",info.total_last_uncomp_size );
	printf( "header_last_uncomp_size = %lu\n", info.header_last_uncomp_size);
	printf( "total_last_comp_size = %lu\n", info.total_last_comp_size);
	printf( "header_last_comp_size = %lu\n", info.header_last_comp_size);
}

bool RohcCompression::init()
{
	if(!initCompressor())
	{
		printf( "ROHC compressor failed to initialize!\n");
		return false;
	}
	if(!initDecompressor())
	{
		printf( "ROHC decompressor failed to initialize!\n");
	   return false;
	}
	return true;
}

bool RohcCompression::initCompressor()
{
	printf("create the ROHC compressor\n");
	_compressor = rohc_comp_new2(ROHC_SMALL_CID, ROHC_SMALL_CID_MAX, gen_random_num, nullptr);
	if (_compressor == nullptr)
	{
		fprintf(stderr, "failed create the ROHC compressor\n");
		return false;
	}
	
	/* enable the UNCOMPRESSED compression profile */
//	printf("enable the IP-only compression profile\n");
//	if (!rohc_comp_enable_profile(_compressor, ROHC_PROFILE_UNCOMPRESSED))
//	{
//		fprintf(stderr, "failed to enable the IP-only profile\n");
//		/* cleanup compressor, then leave with an error code */
//		rohc_comp_free(_compressor);
//		return false ;
//	}
    /* enable the IP-only compression profile */
//    printf("enable the IP-only compression profile\n");
//    if (!rohc_comp_enable_profile(_compressor, ROHC_PROFILE_IP))
//    {
//        fprintf(stderr, "failed to enable the IP-only profile\n");
//        /* cleanup compressor, then leave with an error code */
//        rohc_comp_free(_compressor);
//        return false ;
//    }
    /* enable the TCP-IP compression profile */
    printf("enable the IP-only compression profile\n");
    if (!rohc_comp_enable_profile(_compressor, ROHC_PROFILE_TCP))
    {
        fprintf(stderr, "failed to enable the IP-only profile\n");
        /* cleanup compressor, then leave with an error code */
        rohc_comp_free(_compressor);
        return false ;
    }
	//we dont have a function that would calculate ip tcp checksum
//    printf("enable the NO_IP_CHECKSUMS feature\n");
//    if (!rohc_comp_set_features(_compressor, ROHC_COMP_FEATURE_NO_IP_CHECKSUMS))
//    {
//        fprintf(stderr, "failed to enable features\n");
//    }
	return true;
}

bool RohcCompression::initDecompressor()
{
	//! [create ROHC decompressor #1]
	/* Create a ROHC decompressor to operate:
	 *  - with large CIDs,
	 *  - with the maximum of 5 streams (MAX_CID = 4),
	 *  - in Unidirectional mode (U-mode).
	 */
	//! [create ROHC decompressor #1]
	printf("create the ROHC decompressor\n");
	_decompressor = rohc_decomp_new2(ROHC_SMALL_CID, ROHC_SMALL_CID_MAX, ROHC_U_MODE);
	if(_decompressor == NULL)
	{
		fprintf(stderr, "failed create the ROHC decompressor\n");
		//goto error;
		return false;
	}
	/* Enable the decompression profiles you need */
	printf("enable several ROHC decompression profiles\n");
	//! [enable ROHC decompression profile]
	//    if(!rohc_decomp_enable_profile(decompressor, ROHC_PROFILE_UNCOMPRESSED))
	//    {
	//        fprintf(stderr, "failed to enable the Uncompressed profile\n");
	//        //goto release_decompressor;
	//        rohc_decomp_free(decompressor);
	//        return false;
	//    }
	
//	if(!rohc_decomp_enable_profile(_decompressor, ROHC_PROFILE_UNCOMPRESSED))
//	{
//		fprintf(stderr, "failed to enable the UNCOMPRESSED profile\n");
//		rohc_decomp_free(_decompressor);
//		return false;
//	}
//    if(!rohc_decomp_enable_profile(_decompressor, ROHC_PROFILE_IP))
//    {
//        fprintf(stderr, "failed to enable the IP-only profile\n");
//        rohc_decomp_free(_decompressor);
//        return false;
//    }
    if(!rohc_decomp_enable_profile(_decompressor, ROHC_PROFILE_TCP))
    {
        fprintf(stderr, "failed to enable the TCP-IP profile\n");
        rohc_decomp_free(_decompressor);
        return false;
    }
	//! [enable ROHC decompression profile]
	//! [enable ROHC decompression profiles]
	//    if(!rohc_decomp_enable_profiles(decompressor, ROHC_PROFILE_UDP,
	//                                    ROHC_PROFILE_UDPLITE, -1))
	//    {
	//        fprintf(stderr, "failed to enable the IP/UDP and IP/UDP-Lite "
	//                "profiles\n");
	//        //goto release_decompressor;
	//        rohc_decomp_free(decompressor);
	//        return false;
	//    }
	//! [enable ROHC decompression profiles]
	//we dont have a function that would calculate ip tcp checksum
//	printf("tryout decomp features feature\n");
//	if (!rohc_decomp_set_features(_decompressor, /*ROHC_DECOMP_FEATURE_CRC_REPAIR))*/ROHC_DECOMP_FEATURE_DUMP_PACKETS))
//	{
//		fprintf(stderr, "failed to enable features\n");
//	}
	return true;
}

void RohcCompression::free()
{
	//! [destroy ROHC compressor and decompressor]
	printf("destroy the ROHC compressor\n");
	rohc_comp_free(_compressor);
	printf("destroy the ROHC decompressor\n");
	rohc_decomp_free(_decompressor);
	//! [destroy ROHC compressor and decompressor]
}


#define rohc_buf_init_empty(__data, __max_len) \
{ \
.time = { .sec = 0, .nsec = 0, }, \
.data = (__data), \
.max_len = (__max_len), \
.offset = 0, \
.len = 0, \
}

// IP header's structure
struct ipheader {
    unsigned char      iph_ihl:5, /* Little-endian */
iph_ver:4;
    unsigned char      iph_tos;
    unsigned short int iph_len;
    unsigned short int iph_ident;
    unsigned char      iph_flags;
    unsigned short int iph_offset;
    unsigned char      iph_ttl;
    unsigned char      iph_protocol;
    unsigned short int iph_chksum;
    unsigned int       iph_sourceip;
    unsigned int       iph_destip;
};

/* Structure of a TCP header */
struct tcpheader {
    unsigned short int tcph_srcport;
    unsigned short int tcph_destport;
    unsigned int       tcph_seqnum;
    unsigned int       tcph_acknum;
    unsigned char      tcph_reserved:4, tcph_offset:4;
    // unsigned char tcph_flags;
    unsigned int
tcp_res1:4,       /*little-endian*/
tcph_hlen:4,      /*length of tcp header in 32-bit words*/
tcph_fin:1,       /*Finish flag "fin"*/
tcph_syn:1,       /*Synchronize sequence numbers to start a connection*/
tcph_rst:1,       /*Reset flag */
tcph_psh:1,       /*Push, sends data to the application*/
tcph_ack:1,       /*acknowledge*/
tcph_urg:1,       /*urgent pointer*/
tcph_res2:2;
    unsigned short int tcph_win;
    unsigned short int tcph_chksum;
    unsigned short int tcph_urgptr;
};

void RohcCompression::createIpPacket(char* payload, rohc_buf *ip_packet)
{
//    uint8_t ip_buffer[BUFFER_SIZE]; /* the buffer that will contain the IPv4 packet */
//    /*struct rohc_buf*/ //ip_packet = rohc_buf_init_empty(ip_buffer, BUFFER_SIZE); /* the packet that will contain the IPv4 */
//    ip_packet->time.sec = 0;
//    ip_packet->time.nsec = 0;
//    ip_packet->data = ip_buffer;
//    ip_packet->max_len = BUFFER_SIZE;
//    ip_packet->offset = 0;
//    ip_packet->len = 0;
	
	//ip_packet = { {0,0}, ip_buffer, BUFFER_SIZE ,0 ,0 }; //works on VC++
	//struct iphdr* ip_header; /* the header of the IPv4 packet */
	
	/* create a fake IP packet for the purpose of this simple program */
	//auto ip_header = reinterpret_cast<struct iphdr *>(rohc_buf_data(ip_packet));
	auto ip_header = reinterpret_cast<struct iphdr *>(ip_packet->data + ip_packet->offset + 0);
	ip_header->version = 4; /* we create an IP header version 4 */
	ip_header->ihl = 5; /* min. IPv4 header length (in 32-bit words) */
	ip_packet->len += ip_header->ihl * 4;
	ip_header->tos = 0; /* TOS is not important for the example */
	ip_header->tot_len = htons(ip_packet->len + strlen(FAKE_PAYLOAD));
	ip_header->id = 0; /* ID is not important for the example */
	ip_header->frag_off = 0; /* No packet fragmentation */
	ip_header->ttl = 1; /* TTL is not important for the example */
	ip_header->protocol = 134; /* protocol number */
	ip_header->check =  0x3fa9;//0x3fa9; /* checksum for hello world payload (19 chars)*/
	
	//#if (CC_TARGET_PLATFORM != CC_PLATFORM_WIN32)
	ip_header->saddr = htonl(0x01020304); /* source address 1.2.3.4 */ //unresolved external VC++
	ip_header->daddr = htonl(0x05060708); /* destination addr. 5.6.7.8 */ //unresolved external VC++
	
	//ip_packet->data = (uint8_t)ip_header;
	//#else
	//ip_header->saddr = static_cast<LPIN_ADDR>(0x01020304);
	//ip_header->daddr = *((LPIN_ADDR)*(0x05060708));
	//#endif
	
    //test tcp header
//    char tcp_buffer[BUFFER_SIZE];
//    struct tcpheader *tcp = reinterpret_cast<struct tcpheader *> (tcp_buffer);
//    // The TCP structure. The source port, spoofed, we accept through the command line
//    tcp->tcph_srcport = htons(atoi("1337"));
//    // The destination port, we accept through command line
//    tcp->tcph_destport = htons(atoi("1337"));
//    tcp->tcph_seqnum = htonl(1);
//    tcp->tcph_acknum = 0;
//    tcp->tcph_offset = 5;
//    tcp->tcph_syn = 1;
//    tcp->tcph_ack = 0;
//    tcp->tcph_win = htons(32767);
//    tcp->tcph_chksum = 0; // Done by kernel
//    tcp->tcph_urgptr = 0;
    
	/* copy the payload just after the IP header */
	//rohc_buf_append(&ip_packet, reinterpret_cast<uint8_t *>(FAKE_PAYLOAD), strlen(FAKE_PAYLOAD));
    printf("IP header length: %d \n", ip_packet->len);
    //printf("TCP header length: %d \n", sizeof(tcpheader));
    //rohc_buf_append(ip_packet, (uint8_t *)tcp_buffer, sizeof(tcpheader));
    
    printf("Payload length: %d \n", strlen(FAKE_PAYLOAD));
	rohc_buf_append(ip_packet, (uint8_t *)FAKE_PAYLOAD, strlen(FAKE_PAYLOAD));
	//rohc_buf *pointer = ip_packet;
	//return &ip_packet;
}

// Simple checksum function, may use others such as Cyclic Redundancy Check, CRC
unsigned short RohcCompression::csum(unsigned short *buf, int len)
{
    unsigned long sum;
    for(sum=0; len>0; len--)
        sum += *buf++;
    sum = (sum >> 16) + (sum &0xffff);
    sum += (sum >> 16);
    return (unsigned short)(~sum);
}

void RohcCompression::createRohcPacketTcpIp(char* payload, rohc_buf *ip_packet)
{
    char buffer[BUFFER_SIZE];
    // The size of the headers
    struct ipheader *ip = (struct ipheader *) buffer;
    struct tcpheader *tcp = (struct tcpheader *) (buffer + sizeof(struct ipheader));
    
    memset(buffer, 0, BUFFER_SIZE);
    
    //struct ipheader *ip = reinterpret_cast<struct ipheader *> (ip_packet->data);
    //struct tcpheader *tcp = reinterpret_cast<struct tcpheader *> (ip_packet->data + sizeof(struct ipheader));
    
    // IP structure
    ip->iph_ihl = 5;
    ip->iph_ver = 4;
    ip->iph_tos = 16;
    ip->iph_len = sizeof(struct ipheader) + sizeof(struct tcpheader);
    ip->iph_ident = htons(54321);
    ip->iph_offset = 0;
    ip->iph_ttl = 64;
    ip->iph_protocol = 6; // TCP
    ip->iph_chksum = 0; // Done by kernel
    
    // Source IP, modify as needed, spoofed, we accept through command line argument
    ip->iph_sourceip = inet_addr("1.2.3.4");
    // Destination IP, modify as needed, but here we accept through command line argument
    ip->iph_destip = inet_addr("5.6.7.8");
    
    // The TCP structure. The source port, spoofed, we accept through the command line
    tcp->tcph_srcport = htons(atoi("1337"));
    // The destination port, we accept through command line
    tcp->tcph_destport = htons(atoi("1337"));
    tcp->tcph_seqnum = htonl(1);
    tcp->tcph_acknum = 0;
    tcp->tcph_offset = 5;
    tcp->tcph_syn = 1;
    tcp->tcph_ack = 0;
    tcp->tcph_win = htons(32767);
    tcp->tcph_chksum = 0; // Done by kernel
    tcp->tcph_urgptr = 0;
    // IP checksum calculation
    ip->iph_chksum = csum((unsigned short *) buffer, (sizeof(struct ipheader) + sizeof(struct tcpheader)));

    
    printf("IP header length: %d \n", sizeof(struct ipheader));
    //rohc_buf_append(ip_packet, (uint8_t *)ip, sizeof(struct ipheader));
//    memcpy(rohc_buf_data_at(*ip_packet, ip_packet->len), (uint8_t *)ip,
//           sizeof(struct ipheader));
//    ip_packet->len += sizeof(struct ipheader);
    
    printf("TCP header length: %d \n", sizeof(struct tcpheader));
    //rohc_buf_append(ip_packet, (uint8_t *)tcp, sizeof(struct tcpheader));
//    memcpy(rohc_buf_data_at(*ip_packet, ip_packet->len), (uint8_t *)tcp,
//           sizeof(struct tcpheader));
//    ip_packet->len += sizeof(struct tcpheader);
    
    rohc_buf_append(ip_packet, (uint8_t *)buffer, sizeof(struct ipheader) + sizeof(struct tcpheader));
    
    printf("Payload length: %d \n", strlen(payload));
    rohc_buf_append(ip_packet, (uint8_t *)payload, strlen(payload));
//    memcpy(rohc_buf_data_at(*ip_packet, ip_packet->len), FAKE_PAYLOAD,
//           strlen(FAKE_PAYLOAD));
//    ip_packet->len += strlen(FAKE_PAYLOAD);
}

void printErrorStatus(rohc_status_t status)
{
	switch (status) {
		case ROHC_STATUS_BAD_CRC:
			printf("ROHC_STATUS_BAD_CRC");
			break;
		case ROHC_STATUS_ERROR:
			printf("ROHC_STATUS_ERROR");
			break;
		case ROHC_STATUS_MALFORMED:
			printf("ROHC_STATUS_MALFORMED");
			break;
		case ROHC_STATUS_NO_CONTEXT:
			printf("ROHC_STATUS_NO_CONTEXT");
			break;
		case ROHC_STATUS_OUTPUT_TOO_SMALL:
			printf("ROHC_STATUS_OUTPUT_TOO_SMALL");
			break;
		case ROHC_STATUS_SEGMENT:
			printf("ROHC_STATUS_SEGMENT");
			break;
		default:
			printf("UNKNOWN ERROR");
			break;
	}
}

bool RohcCompression::compress(struct rohc_buf *const ip_packet, struct rohc_buf *rohc_packet)
{
	//uint8_t rohc_buffer[BUFFER_SIZE];
	//rohc_packet = rohc_buf_init_empty(rohc_buffer, BUFFER_SIZE);  /* the packet that will contain the resulting ROHC packet */
	
	//auto ip_packet2 = createIpPacket((char*)FAKE_PAYLOAD);
	//struct rohc_buf ip_packet2;
	//createIpPacket((char*)FAKE_PAYLOAD, ip_packet2);
	/* compress the fake IP packet */
	struct rohc_buf ip_packet3(*ip_packet);
	auto status = rohc_compress4(_compressor, ip_packet3, rohc_packet);
	if (status != ROHC_STATUS_OK)
	{
		printErrorStatus(status);
		fprintf(stderr, " - compression failed: %s (%d)\n",
				rohc_strerror(status), status);
		return false;
	}
	return true;
}

bool RohcCompression::decompress(struct rohc_buf *const rohc_packet2, struct rohc_buf &ip_packet)
{
	/* the buffer that will contain the resulting IP packet */
	unsigned char ip_buffer[BUFFER_SIZE];
	ip_packet = rohc_buf_init_empty(ip_buffer, BUFFER_SIZE);
	/* we do not want to handle feedback in this simple example */
	struct rohc_buf *rcvd_feedback = NULL;
	struct rohc_buf *feedback_send = NULL;
	
	/* Now, decompress this fake ROHC packet */
	//printf("decompress the fake ROHC packet\n");
	//rohc_buf rohc_packet2;
	//rohc_buf ip_packet2;
	//if(!compress(ip_packet2, rohc_packet2)) return false;
	rohc_buf rohc_packet(*rohc_packet2);
	auto status = rohc_decompress3(_decompressor, rohc_packet, &ip_packet,
							  rcvd_feedback, feedback_send);
	if(status == ROHC_STATUS_OK)
	{
		/* decompression is successful */
		if(!rohc_buf_is_empty(ip_packet))
		{
			/* ip_packet.len bytes of decompressed IP data available in
			 * ip_packet: dump the IP packet on the standard output */
			//printf("IP packet resulting from the ROHC decompression:\n");
			//dump_packet(ip_packet);
			//print payload
//            auto dest = new unsigned char[ip_packet.len];
//            std::string res = "";
//            for (int i = 0; i < ip_packet.len; i++) {
//                res += dest[i];
//            }
		}
		else
		{
			/* no IP packet was decompressed because of ROHC segmentation or
			 * feedback-only packet:
			 *  - the ROHC packet was a non-final segment, so at least another
			 *    ROHC segment is required to be able to decompress the full
			 *    ROHC packet
			 *  - the ROHC packet was a feedback-only packet, it contained only
			 *    feedback information, so there was nothing to decompress */
			printf("no IP packet decompressed");
		}
	}
	else
	{
		printErrorStatus(status);
		/* failure: decompressor failed to decompress the ROHC packet */
		fprintf(stderr, " - decompression failed\n");
		return false;
	}
	
	return true;
}

// Computing the internet checksum (RFC 1071).
uint16_t
checksum (uint16_t *addr, int len)
{
    int count = len;
    uint32_t sum = 0;
    uint16_t answer = 0;
    
    // Sum up 2-byte values until none or only one byte left.
    while (count > 1) {
        sum += *(addr++);
        count -= 2;
    }
    
    // Add left-over byte, if any.
    if (count > 0) {
        sum += *(uint8_t *) addr;
    }
    
    // Fold 32-bit sum into 16 bits; we lose information by doing this,
    // increasing the chances of a collision.
    // sum = (lower 16 bits) + (upper 16 bits shifted right 16 bits)
    while (sum >> 16) {
        sum = (sum & 0xffff) + (sum >> 16);
    }
    
    // Checksum is one's compliment of sum.
    answer = ~sum;
    
    return (answer);
}

// Build IPv4 TCP pseudo-header and call checksum function.
uint16_t
tcp4_checksum (struct ip iphdr, struct tcphdr tcphdr)
{
    uint16_t svalue;
    char buf[IP_MAXPACKET], cvalue;
    char *ptr;
    int chksumlen = 0;
    
    // ptr points to beginning of buffer buf
    ptr = &buf[0];
    
    // Copy source IP address into buf (32 bits)
    memcpy (ptr, &iphdr.ip_src.s_addr, sizeof (iphdr.ip_src.s_addr));
    ptr += sizeof (iphdr.ip_src.s_addr);
    chksumlen += sizeof (iphdr.ip_src.s_addr);
    
    // Copy destination IP address into buf (32 bits)
    memcpy (ptr, &iphdr.ip_dst.s_addr, sizeof (iphdr.ip_dst.s_addr));
    ptr += sizeof (iphdr.ip_dst.s_addr);
    chksumlen += sizeof (iphdr.ip_dst.s_addr);
    
    // Copy zero field to buf (8 bits)
    *ptr = 0; ptr++;
    chksumlen += 1;
    
    // Copy transport layer protocol to buf (8 bits)
    memcpy (ptr, &iphdr.ip_p, sizeof (iphdr.ip_p));
    ptr += sizeof (iphdr.ip_p);
    chksumlen += sizeof (iphdr.ip_p);
    
    // Copy TCP length to buf (16 bits)
    svalue = htons (sizeof (tcphdr));
    memcpy (ptr, &svalue, sizeof (svalue));
    ptr += sizeof (svalue);
    chksumlen += sizeof (svalue);
    
    // Copy TCP source port to buf (16 bits)
    memcpy (ptr, &tcphdr.th_sport, sizeof (tcphdr.th_sport));
    ptr += sizeof (tcphdr.th_sport);
    chksumlen += sizeof (tcphdr.th_sport);
    
    // Copy TCP destination port to buf (16 bits)
    memcpy (ptr, &tcphdr.th_dport, sizeof (tcphdr.th_dport));
    ptr += sizeof (tcphdr.th_dport);
    chksumlen += sizeof (tcphdr.th_dport);
    
    // Copy sequence number to buf (32 bits)
    memcpy (ptr, &tcphdr.th_seq, sizeof (tcphdr.th_seq));
    ptr += sizeof (tcphdr.th_seq);
    chksumlen += sizeof (tcphdr.th_seq);
    
    // Copy acknowledgement number to buf (32 bits)
    memcpy (ptr, &tcphdr.th_ack, sizeof (tcphdr.th_ack));
    ptr += sizeof (tcphdr.th_ack);
    chksumlen += sizeof (tcphdr.th_ack);
    
    // Copy data offset to buf (4 bits) and
    // copy reserved bits to buf (4 bits)
    cvalue = (tcphdr.th_off << 4) + tcphdr.th_x2;
    memcpy (ptr, &cvalue, sizeof (cvalue));
    ptr += sizeof (cvalue);
    chksumlen += sizeof (cvalue);
    
    // Copy TCP flags to buf (8 bits)
    memcpy (ptr, &tcphdr.th_flags, sizeof (tcphdr.th_flags));
    ptr += sizeof (tcphdr.th_flags);
    chksumlen += sizeof (tcphdr.th_flags);
    
    // Copy TCP window size to buf (16 bits)
    memcpy (ptr, &tcphdr.th_win, sizeof (tcphdr.th_win));
    ptr += sizeof (tcphdr.th_win);
    chksumlen += sizeof (tcphdr.th_win);
    
    // Copy TCP checksum to buf (16 bits)
    // Zero, since we don't know it yet
    *ptr = 0; ptr++;
    *ptr = 0; ptr++;
    chksumlen += 2;
    
    // Copy urgent pointer to buf (16 bits)
    memcpy (ptr, &tcphdr.th_urp, sizeof (tcphdr.th_urp));
    ptr += sizeof (tcphdr.th_urp);
    chksumlen += sizeof (tcphdr.th_urp);
    
    return checksum ((uint16_t *) buf, chksumlen);
}

// Allocate memory for an array of chars.
char *
allocate_strmem (int len)
{
    void *tmp;
    
    if (len <= 0) {
        fprintf (stderr, "ERROR: Cannot allocate memory because len = %i in allocate_strmem().\n", len);
        exit (EXIT_FAILURE);
    }
    
    tmp = (char *) malloc (len * sizeof (char));
    if (tmp != NULL) {
        memset (tmp, 0, len * sizeof (char));
        return (char *)(tmp);
    } else {
        fprintf (stderr, "ERROR: Cannot allocate memory for array allocate_strmem().\n");
        exit (EXIT_FAILURE);
    }
}

// Allocate memory for an array of unsigned chars.
uint8_t *
allocate_ustrmem (int len)
{
    void *tmp;
    
    if (len <= 0) {
        fprintf (stderr, "ERROR: Cannot allocate memory because len = %i in allocate_ustrmem().\n", len);
        exit (EXIT_FAILURE);
    }
    
    tmp = (uint8_t *) malloc (len * sizeof (uint8_t));
    if (tmp != NULL) {
        memset (tmp, 0, len * sizeof (uint8_t));
        return (uint8_t *)(tmp);
    } else {
        fprintf (stderr, "ERROR: Cannot allocate memory for array allocate_ustrmem().\n");
        exit (EXIT_FAILURE);
    }
}

// Allocate memory for an array of ints.
int *
allocate_intmem (int len)
{
    void *tmp;
    
    if (len <= 0) {
        fprintf (stderr, "ERROR: Cannot allocate memory because len = %i in allocate_intmem().\n", len);
        exit (EXIT_FAILURE);
    }
    
    tmp = (int *) malloc (len * sizeof (int));
    if (tmp != NULL) {
        memset (tmp, 0, len * sizeof (int));
        return (int *)(tmp);
    } else {
        fprintf (stderr, "ERROR: Cannot allocate memory for array allocate_intmem().\n");
        exit (EXIT_FAILURE);
    }
}

#endif
