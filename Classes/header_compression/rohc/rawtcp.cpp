#include "rawtcp.h"
 
/*
    Generic checksum calculation function
*/
unsigned short RawTcp::csum(unsigned short *ptr,int nbytes)
{
    long sum;
    unsigned short oddbyte;
    short answer;
 
    sum=0;
    while(nbytes>1) {
        sum+=*ptr++;
        nbytes-=2;
    }
    if(nbytes==1) {
        oddbyte=0;
        *((u_char*)&oddbyte)=*(u_char*)ptr;
        sum+=oddbyte;
    }
 
    sum = (sum>>16)+(sum & 0xffff);
    sum = sum + (sum>>16);
    answer=(short)~sum;
     
    return(answer);
}

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

char* RawTcp::createRawTcpPacket(int &packetLength)
{
    //Create a raw socket
//    int s = socket (PF_INET, SOCK_RAW, IPPROTO_TCP);
//     
//    if(s == -1)
//    {
//        //socket creation failed, may be because of non-root privileges
//        perror("Failed to create socket");
//        exit(1);
//    }
    
    //Datagram to represent the packet
    char datagram[4096] , source_ip[32] , *data , *pseudogram;
     
    //zero out the packet buffer
    memset (datagram, 0, 4096);
     
    //IP header
    struct iphdr *iph = (struct iphdr *) datagram;
     
    //TCP header
    struct tcphdr *tcph = (struct tcphdr *) (datagram + sizeof (struct ip));
    struct sockaddr_in sin;
    struct pseudo_header psh;
     
    //Data part
    data = datagram + sizeof(struct iphdr) + sizeof(struct tcphdr);
    strcpy(data , "ABCDEFGHIJKLMNOPQRSTUVWXYZ");
     
    //some address resolution
//    strcpy(source_ip , "192.168.1.2");
//    sin.sin_family = AF_INET;
//    sin.sin_port = htons(80);
//    sin.sin_addr.s_addr = inet_addr ("1.2.3.4");
    
    //Fill in the IP Header
    iph->ihl = 5;
    iph->version = 4;
    iph->tos = 0;
    iph->tot_len = sizeof (struct iphdr) + sizeof (struct tcphdr) + strlen(data);
    iph->id = 0;/* ID is not important for the example */ //htonl (54321); //Id of this packet
    iph->frag_off = 0;
    iph->ttl = 255;
    iph->protocol = IPPROTO_TCP;
    iph->check = 0;      //Set to 0 before calculating checksum
    iph->saddr = inet_addr ( source_ip );    //Spoof the source ip address
    iph->daddr = sin.sin_addr.s_addr;
    iph->saddr = htonl(0x01020304); /* source address 1.2.3.4 */
    iph->daddr = htonl(0x05060708); /* destination addr. 5.6.7.8 */
    
    //Ip checksum
    iph->check = csum ((unsigned short *) datagram, iph->tot_len);
     
    //TCP Header
    tcph->th_sport = htons (1234); //source
    tcph->th_dport = htons (80); //dest
    tcph->th_seq = 0; //seq
    tcph->th_ack = 0; //ack_seq
    tcph->th_off = 5;  //tcp header size //doff
//    tcph->fin=0; //fin
//    tcph->syn=1; //syn
//    tcph->rst=0; //rst
//    tcph->psh=0; //psh
//    tcph->ack=0; //ack
//    tcph->urg=0; //urg
    tcph->th_flags = TH_SYN;
    tcph->th_win = htons (5840); /* maximum allowed window size */
    tcph->th_sum = 0; //leave checksum 0 now, filled later by pseudo header
    tcph->th_urp = 0; //urg_ptr
     
    //Now the TCP checksum
    psh.source_address = inet_addr( source_ip );
    psh.dest_address = sin.sin_addr.s_addr;
    psh.placeholder = 0;
    psh.protocol = IPPROTO_TCP;
    psh.tcp_length = htons(sizeof(struct tcphdr) + strlen(data) );
     
    int psize = sizeof(struct pseudo_header) + sizeof(struct tcphdr) + strlen(data);
    pseudogram = (char *)malloc(psize);
     
    memcpy(pseudogram , (char*) &psh , sizeof (struct pseudo_header));
    memcpy(pseudogram + sizeof(struct pseudo_header) , tcph , sizeof(struct tcphdr) + strlen(data));
     
    tcph->th_sum = csum( (unsigned short*) pseudogram , psize);
    
    packetLength = iph->tot_len;
    return datagram;
    //IP_HDRINCL to tell the kernel that headers are included in the packet
//    int one = 1;
//    const int *val = &one;
//     
//    if (setsockopt (s, IPPROTO_IP, IP_HDRINCL, val, sizeof (one)) < 0)
//    {
//        perror("Error setting IP_HDRINCL");
//        exit(0);
//    }
//     
//    //loop if you want to flood :)
//    while (1)
//    {
//        //Send the packet
//        if (sendto (s, datagram, iph->tot_len ,  0, (struct sockaddr *) &sin, sizeof (sin)) < 0)
//        {
//            perror("sendto failed");
//        }
//        //Data send successfully
//        else
//        {
//            printf ("Packet Send. Length : %d \n" , iph->tot_len);
//        }
//    }
//     
//    return 0;
}
 
//Complete
