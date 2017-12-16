//int main(int argc, char **argv)
//{
//    struct rohc_comp *compressor; /* the ROHC compressor */
//    struct rohc_decomp *decompressor;
//    unsigned char ip_packet[BUFFER_SIZE]; /* the buffer that will contain
//                                           the IPv4 packet to compress */
//    unsigned int ip_packet_len; /* the length (in bytes) of the
//                                 IPv4 packet */
//    unsigned char rohc_packet[BUFFER_SIZE]; /* the buffer that will contain
//                                             the resulting ROHC packet */
//    int rohc_packet_len; /* the length (in bytes) of the
//                          resulting ROHC packet */
//    unsigned int seed;
//    unsigned int i;
//    
//    unsigned char udp_packet[BUFFER_SIZE];
//    unsigned int udp_packet_len;
//    unsigned char rtp_header[BUFFER_SIZE];
//    unsigned char decomp_packet[BUFFER_SIZE];
//    
//    /* initialize the random generator */
//    seed = time(NULL);
//    srand(seed);
//    
//    /* Create a ROHC compressor with small CIDs, no jamming and no adaptation
//     * to encapsulation frames.
//     *
//     * See http://rohc-lib.org/doc/latest/group__rohc__comp.html#ga721fd34fc0cd9e1d789b693eb6bb6485
//     * for details about rohc_alloc_compressor in the API documentation.
//     */
//    printf("\ncreate the ROHC compressor\n");
//    compressor = rohc_alloc_compressor(ROHC_SMALL_CID_MAX, 0, 0, 0);
//    if(compressor == NULL)
//    {
//        fprintf(stderr, "failed create the ROHC compressor\n");
//        goto error;
//    }
//    decompressor = rohc_alloc_decompressor(compressor);
//    
//    /* set the callback for random numbers */
//    if(!rohc_comp_set_random_cb(compressor, gen_random_num, NULL))
//    {
//        fprintf(stderr, "failed to set the callback for random numbers\n");
//        goto release_compressor;
//    }
//    
//    /* Enable the compression profiles you need (comment or uncomment some lines).
//     *
//     * See http://rohc-lib.org/doc/latest/group__rohc__comp.html#ga1a444eb91681521f726712a60a4df867
//     * for details about rohc_activate_profile in the API documentation.
//     */
//    printf("\nenable several ROHC compression profiles\n");
//    rohc_activate_profile(compressor, ROHC_PROFILE_UNCOMPRESSED);
//    rohc_activate_profile(compressor, ROHC_PROFILE_UDP);
//    rohc_activate_profile(compressor, ROHC_PROFILE_IP);
//    //rohc_activate_profile(compressor, ROHC_PROFILE_UDPLITE);
//    rohc_activate_profile(compressor, ROHC_PROFILE_RTP);
//    //rohc_activate_profile(comp, ROHC_PROFILE_ESP);
//    
//    /* create a fake IP packet for the purpose of this simple program */
//    printf("\nbuild a fake IP packet\n");
//    ip_packet[0] = 4 << 4; /* IP version 4 */
//    ip_packet[0] |= 5; /* IHL: minimal IPv4 header length (in 32-bit words) */
//    ip_packet[1] = 0; /* TOS */
//    ip_packet_len = 56; //5 * 4 + strlen(FAKE_PAYLOAD);
//    ip_packet[2] = (htons(ip_packet_len) >> 8) & 0xff; /* Total Length */
//    ip_packet[3] = htons(ip_packet_len) & 0xff;
//    //ip_packet[2] = (ip_packet_len >> 8) & 0xff; /* Total Length */
//    //ip_packet[3] = ip_packet_len & 0xff;
//    ip_packet[4] = 0; /* IP-ID */
//    ip_packet[5] = 0;
//    ip_packet[6] = 0; /* Fragment Offset and IP flags */
//    ip_packet[7] = 0;
//    ip_packet[8] = 1; /* TTL */
//    ip_packet[9] = 17; /* Protocol: unassigned number */
//    ip_packet[10] = 0xbe; /* fake Checksum */
//    ip_packet[11] = 0xef;
//    ip_packet[12] = 0x01; /* Source address */
//    ip_packet[13] = 0x02;
//    ip_packet[14] = 0x03;
//    ip_packet[15] = 0x04;
//    ip_packet[16] = 0x05; /* Destination address */
//    ip_packet[17] = 0x06;
//    ip_packet[18] = 0x07;
//    ip_packet[19] = 0x08;
//    
//    /* fake UDP headers, 8 bytes */
//    udp_packet[0] = 0x01; /* Source address */
//    udp_packet[1] = 0x02;
//    udp_packet[2] = 0x05; /* Destination address */
//    udp_packet[3] = 0x06;
//    udp_packet_len = 8;
//    udp_packet[4] = (udp_packet_len >> 8) & 0xff; /* Total Length */
//    udp_packet[5] = udp_packet_len & 0xff;
//    udp_packet[6] = 0xbe; /* fake Checksum */
//    udp_packet[7] = 0xef;
//    
//    /* fake RTP headers, 12 bytes, and payload 16 bytes */
//    rtp_header[ 0 ] = 0x80;
//    rtp_header[ 1 ] = 0x1;
//    rtp_header[ 2 ] = 0xb9;
//    rtp_header[ 3 ] = 0x40;
//    rtp_header[ 4 ] = 0x4b;
//    rtp_header[ 5 ] = 0x82;
//    rtp_header[ 6 ] = 0xe5;
//    rtp_header[ 7 ] = 0x6c;
//    rtp_header[ 8 ] = 0x50;
//    rtp_header[ 9 ] = 0x68;
//    rtp_header[ 10 ] = 0xe5;
//    rtp_header[ 11 ] = 0x2b;
//    rtp_header[ 12 ] = 0xc;
//    rtp_header[ 13 ] = 0x3e;
//    rtp_header[ 14 ] = 0x8e;
//    rtp_header[ 15 ] = 0x4a;
//    rtp_header[ 16 ] = 0x69;
//    rtp_header[ 17 ] = 0x52;
//    rtp_header[ 18 ] = 0x1b;
//    rtp_header[ 19 ] = 0xfa;
//    rtp_header[ 20 ] = 0x2;
//    rtp_header[ 21 ] = 0xe0;
//    rtp_header[ 22 ] = 0xb0;
//    rtp_header[ 23 ] = 0x37;
//    rtp_header[ 24 ] = 0x9f;
//    rtp_header[ 25 ] = 0xd8;
//    rtp_header[ 26 ] = 0x74;
//    rtp_header[ 27 ] = 0x10;
//    
//    /* copy the payload just after the IP header */
//    //memcpy(ip_packet + 5 * 4, FAKE_PAYLOAD, strlen(FAKE_PAYLOAD));
//    memcpy(ip_packet + 20, udp_packet, 8);
//    memcpy(ip_packet + 28, rtp_header, 28);
//    
//    /* dump the newly-created IP packet on terminal */
//    for(i = 0; i < ip_packet_len; i++)
//    {
//        printf("0x%02x ", ip_packet[i]);
//        if(i != 0 && ((i + 1) % 8) == 0)
//        {
//            printf("\n");
//        }
//    }
//    if(i != 0 && ((i + 1) % 8) != 0) /* be sure to go to the line */
//    {
//        printf("\n");
//    }
//    
//    /* Now, compress this fake IP packet.
//     *
//     * See http://rohc-lib.org/doc/latest/group__rohc__comp.html#ga99be8242b7bc4f442f4519461a99726b
//     * for details about rohc_compress in the API documentation.
//     */
//    for (int j = 0; j < 30; j++ ){
//        printf("\ncompress the fake IP packet, j = %d\n", j);
//        rohc_packet_len = rohc_compress(compressor,
//                                        ip_packet, ip_packet_len,
//                                        rohc_packet, BUFFER_SIZE);
//        if(rohc_packet_len <= 0)
//        {
//            fprintf(stderr, "compression of fake IP packet failed\n");
//            goto release_compressor;
//        }
//        
//        /* dump the ROHC packet on terminal */
//        printf("\nROHC packet resulting from the ROHC compression:\n");
//        for(i = 0; i < ((unsigned int) rohc_packet_len); i++)
//        {
//            printf("0x%02x ", rohc_packet[i]);
//            if(i != 0 && ((i + 1) % 8) == 0)
//            {
//                printf("\n");
//            }
//        }
//        if(i != 0 && ((i + 1) % 8) != 0) /* be sure to go to the line */
//        {
//            printf("\n");
//        }
//        
//        int decomp_size = rohc_decompress(decompressor,
//                                          rohc_packet, rohc_packet_len,
//                                          decomp_packet, BUFFER_SIZE);
//        
//        if(0 == decomp_size)
//        {
//            printf("\ndecompression of fake IP packet failed.\n");
//        }
//        else {
//            printf("\nROHC packet resulting from the ROHC decompression:\n");
//            for (int i = 0; i< decomp_size; i++) {
//                printf("0x%02x ", decomp_packet[i]);
//                if(i != 0 && ((i + 1) % 8) == 0)
//                {
//                    printf("\n");
//                }
//            }
//        }
//        
//    } // end of 10 times loop
