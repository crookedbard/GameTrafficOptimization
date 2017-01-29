//
//  RohcCompression.hpp
//  GameTrafficOptimization
//
//  Created by Dainius Kreivys on 2017-01-15.
//
//
#if (CC_TARGET_PLATFORM != CC_PLATFORM_IOS && CC_TARGET_PLATFORM != CC_PLATFORM_WIN32)
#ifndef RohcCompression_hpp
#define RohcCompression_hpp

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


struct rohc_buf;
struct rohc_comp;
struct rohc_decomp;

class RohcCompression
{
public:
    static bool init(); //initialize compressor and decompressor
    static void free(); //cleanup - free compressor and decompressor
    static bool compress(struct rohc_buf *const ip_packet, struct rohc_buf *rohc_packet); // if success then returns compressed rohc packet
    static bool decompress(struct rohc_buf *const rohc_packet, struct rohc_buf &ip_packet); // if success then returns uncompressed tcp ip packet
    static void performTest(); //creates ip packet, compresses and decompresses it, prints results.
private:
    static bool initCompressor(); //init compressor
    static bool initDecompressor(); //init decompressor
    static void createIpPacket(char* payload, struct rohc_buf *ip_packet); //returns ip packet as rohc buffer
    static struct rohc_comp* _compressor;
    static struct rohc_decomp* _decompressor;
    static void printLastCompressedPacketInfo();
    static unsigned short csum(unsigned short *buf, int len);
    static void createRohcPacketTcpIp(char* payload, struct rohc_buf *ip_packet); //returns ip packet as rohc buffer
};

#endif /* RohcCompression_hpp */
#endif
