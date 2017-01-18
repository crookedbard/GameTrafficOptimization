//
//  RohcCompression.cpp
//  GameTrafficOptimization
//
//  Created by Dainius Kreivys on 2017-01-15.
//
//

#include "RohcCompression.hpp"
#include <stdio.h>  /* for the printf() function */
#include <string.h>      /* for the strlen() */
#include <rohc/rohc.h> /* includes required to use the compression part of the ROHC library */
#include <rohc/rohc_buf.h>  /* for the rohc_buf_*() functions */
#include <rohc/rohc_comp.h>   /* for rohc_comp_*() functions */

//gcc libraries
//include <netinet/ip.h>  /* for the IPv4 header */
//#include <time.h> /* required by time() */
//#if (CC_TARGET_PLATFORM != CC_PLATFORM_IOS && CC_TARGET_PLATFORM != CC_PLATFORM_WIN32)
//typedef signed __int64 int64_t;
//typedef signed __int32 int32_t;
//typedef signed __int16 int16_t;
//typedef signed __int8 int8_t;
//
//typedef unsigned __int64 uint64_t;
//typedef unsigned __int32 uint32_t;
//typedef unsigned __int16 uint16_t;
//typedef unsigned __int8 uint8_t;
//typedef unsigned int     size_t;
//#endif
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
#include "transport_protocols\tcp\Common.h"
#else
#include "Common.h"
#endif

/* The size (in bytes) of the buffers used in the program */
#define BUFFER_SIZE 2048

/* The payload for the fake IP packet */
#define FAKE_PAYLOAD "hello, ROHC world!"

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
static int gen_random_num(const struct rohc_comp *const comp,
                          void *const user_context)
{
    return rand();
}

bool RohcCompression::compress(char* payload)
{
    struct rohc_comp *compressor;  /* the ROHC compressor */
    
    /* the buffer that will contain the IPv4 packet to compress */
    uint8_t ip_buffer[BUFFER_SIZE];
    /* the packet that will contain the IPv4 packet to compress */
    struct rohc_buf ip_packet = rohc_buf_init_empty(ip_buffer, BUFFER_SIZE);
    /* the header of the IPv4 packet */
    struct iphdr *ip_header;
    
    uint8_t rohc_buffer[BUFFER_SIZE];
    /* the packet that will contain the resulting ROHC packet */
    struct rohc_buf rohc_packet = rohc_buf_init_empty(rohc_buffer, BUFFER_SIZE);
    
    //#if (CC_TARGET_PLATFORM != CC_PLATFORM_IOS && CC_TARGET_PLATFORM != CC_PLATFORM_WIN32)
    rohc_status_t rohc_status;
    size_t i;
    //#endif
    
    /* print the purpose of the program on the console */
    printf("This program will compress one single IPv4 packet\n");
    
    /* initialize the random generator with the current time */
    srand(time(NULL));
    
    /* create a ROHC compressor with good default parameters */
    printf("create the ROHC compressor\n");
#if (CC_TARGET_PLATFORM != CC_PLATFORM_WIN32)
    compressor = rohc_comp_new2(ROHC_SMALL_CID, ROHC_SMALL_CID_MAX, //unresolved external VC++
                                gen_random_num, NULL);
#endif
    if (compressor == NULL)
    {
        fprintf(stderr, "failed create the ROHC compressor\n");
        /* leave with an error code */
        return false; //"Error";//1;
    }
#if (CC_TARGET_PLATFORM != CC_PLATFORM_WIN32)
    /* enable the IP-only compression profile */
    printf("enable the IP-only compression profile\n");
    if (!rohc_comp_enable_profile(compressor, ROHC_PROFILE_IP))
    {
        fprintf(stderr, "failed to enable the IP-only profile\n");
        /* cleanup compressor, then leave with an error code */
        rohc_comp_free(compressor); //unresolved external VC++
        return false ;//1;
    }
#endif
    /* create a fake IP packet for the purpose of this simple program */
    printf("build a fake IP packet\n");
    ip_header = (struct iphdr *) rohc_buf_data(ip_packet);
    
    ip_header->version = 4; /* we create an IP header version 4 */
    ip_header->ihl = 5; /* min. IPv4 header length (in 32-bit words) */
    ip_packet.len += ip_header->ihl * 4;
    ip_header->tos = 0; /* TOS is not important for the example */
//#if (CC_TARGET_PLATFORM != CC_PLATFORM_WIN32)
    ip_header->tot_len = htons(ip_packet.len + strlen(FAKE_PAYLOAD)); //turi veikt. socket.cpp veikia unresolved external VC++
//#endif
    ip_header->id = 0; /* ID is not important for the example */
    ip_header->frag_off = 0; /* No packet fragmentation */
    ip_header->ttl = 1; /* TTL is not important for the example */
    ip_header->protocol = 134; /* protocol number */
    ip_header->check = 0x3fa9; /* checksum */
    
#if (CC_TARGET_PLATFORM != CC_PLATFORM_WIN32)
    ip_header->saddr = htonl(0x01020304); /* source address 1.2.3.4 */    //unresolved external VC++
    ip_header->daddr = htonl(0x05060708); /* destination addr. 5.6.7.8 */ //unresolved external VC++
#else
    ip_header->saddr = *((LPIN_ADDR)*(0x01020304));
    ip_header->daddr = *((LPIN_ADDR)*(0x05060708));
#endif
    /* copy the payload just after the IP header */
    rohc_buf_append(&ip_packet, (uint8_t *)FAKE_PAYLOAD, strlen(FAKE_PAYLOAD));
    
    /* dump the newly-created IP packet on terminal */
    for (i = 0; i < ip_packet.len; i++)
    {
        printf("0x%02x ", rohc_buf_byte_at(ip_packet, i));
        if (i != 0 && ((i + 1) % 8) == 0)
        {
            printf("\n");
        }
    }
    
    if(i != 0 && (i % 8) != 0) /* be sure to go to the line */
    {
        printf("\n");
    }
    
    /* compress the fake IP packet */
    printf("compress the fake IP packet\n");
#if (CC_TARGET_PLATFORM != CC_PLATFORM_IOS && CC_TARGET_PLATFORM != CC_PLATFORM_WIN32)
    rohc_status = rohc_compress4(compressor, ip_packet, &rohc_packet); //unresolved external VC++
#endif
    if (rohc_status != ROHC_STATUS_OK)
    {
#if (CC_TARGET_PLATFORM != CC_PLATFORM_IOS && CC_TARGET_PLATFORM != CC_PLATFORM_WIN32)
        fprintf(stderr, "compression of fake IP packet failed: %s (%d)\n",
                rohc_strerror(rohc_status), rohc_status);  //unresolved external VC++
        /* cleanup compressor, then leave with an error code */
        rohc_comp_free(compressor); //unresolved external VC++
#endif
        return false; //1;
    }
    
    /* dump the ROHC packet on terminal */
    printf("ROHC packet resulting from the ROHC compression:\n");
    for (i = 0; i < rohc_packet.len; i++)
    {
        printf("0x%02x ", rohc_buf_byte_at(rohc_packet, i));
        if (i != 0 && ((i + 1) % 8) == 0)
        {
            printf("\n");
        }
    }
    if (i != 0 && (i % 8) != 0) /* be sure to go to the line */
    {
        printf("\n");
    }
    
    printf("destroy the ROHC decompressor\n");
#if (CC_TARGET_PLATFORM != CC_PLATFORM_IOS && CC_TARGET_PLATFORM != CC_PLATFORM_WIN32)  
    rohc_comp_free(compressor);  //unresolved external VC++
#endif
    /* leave the program with a success code */
	return true;
}

bool RohcCompression::decompress(char* payload)
{
	return false;
}
