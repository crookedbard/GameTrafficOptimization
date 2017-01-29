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

//printf("%s %d",__FILE__,__LINE__); fflush(stdout);\
//printf("::"); \
//printf(*A,##args); \

/* The size (in bytes) of the buffers used in the program */
#define BUFFER_SIZE 2048

/* The payload for the fake IP packet */
#define FAKE_PAYLOAD "hello, ROHC world!"

#define PACKET_COUNT 10



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

void RohcCompression::performTest()
{
	printf("ROHC version %s\n", rohc_version());
	//srand48(time(nullptr)); /* initialize the random generator with the current time */ //-don't know if it is needed. Why compressor needs random value?
	if(!RohcCompression::init()) return;
	
	uint8_t ip_buffer[BUFFER_SIZE];
	rohc_buf ip_packet = rohc_buf_init_empty(ip_buffer, BUFFER_SIZE);
    //createRohcPacketTcpIp((char*)FAKE_PAYLOAD, &ip_packet);
	createIpPacket((char*)FAKE_PAYLOAD, &ip_packet);
	
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
		//printf("Packet no: %d decompressed Packet Length:\t%zu\n", i, decompressed_ip_packet.len);
	}
	
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
	printf("enable the IP-only compression profile\n");
	if (!rohc_comp_enable_profile(_compressor, ROHC_PROFILE_UNCOMPRESSED))
	{
		fprintf(stderr, "failed to enable the IP-only profile\n");
		/* cleanup compressor, then leave with an error code */
		rohc_comp_free(_compressor);
		return false ;
	}
    /* enable the IP-only compression profile */
    printf("enable the IP-only compression profile\n");
    if (!rohc_comp_enable_profile(_compressor, ROHC_PROFILE_IP))
    {
        fprintf(stderr, "failed to enable the IP-only profile\n");
        /* cleanup compressor, then leave with an error code */
        rohc_comp_free(_compressor);
        return false ;
    }
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
    printf("enable the NO_IP_CHECKSUMS feature\n");
    if (!rohc_comp_set_features(_compressor, ROHC_COMP_FEATURE_NO_IP_CHECKSUMS))
    {
        fprintf(stderr, "failed to enable features\n");
    }
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
	
	if(!rohc_decomp_enable_profile(_decompressor, ROHC_PROFILE_UNCOMPRESSED))
	{
		fprintf(stderr, "failed to enable the UNCOMPRESSED profile\n");
		rohc_decomp_free(_decompressor);
		return false;
	}
    if(!rohc_decomp_enable_profile(_decompressor, ROHC_PROFILE_IP))
    {
        fprintf(stderr, "failed to enable the IP-only profile\n");
        rohc_decomp_free(_decompressor);
        return false;
    }
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
	printf("tryout decomp features feature\n");
	if (!rohc_decomp_set_features(_decompressor, /*ROHC_DECOMP_FEATURE_CRC_REPAIR))*/ROHC_DECOMP_FEATURE_DUMP_PACKETS))
	{
		fprintf(stderr, "failed to enable features\n");
	}
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
#endif
