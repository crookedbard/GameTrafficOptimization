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
    static void createIpPacket(char* payload, struct rohc_buf *ip_packet); //returns normal tcp ip packet
    static struct rohc_comp* _compressor;
    static struct rohc_decomp* _decompressor;
    static void printLastCompressedPacketInfo();
};

#endif /* RohcCompression_hpp */
#endif
