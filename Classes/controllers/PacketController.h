#ifndef __PACKET_CONTROLLER_H__
#define __PACKET_CONTROLLER_H__

#define MSG_FEW_INT 1
#define MSG_FEW_STRING 2
#define MSG_MANY_INT 3
#define MSG_MANY_STRING 4

#define MSG_FEW_INT_LZ 5
#define MSG_FEW_STRING_LZ 6
#define MSG_MANY_INT_LZ 7
#define MSG_MANY_STRING_LZ 8

#define MSG_FEW_INT_HUFFMAN 9
#define MSG_FEW_STRING_HUFFMAN 10
#define MSG_MANY_INT_HUFFMAN 11
#define MSG_MANY_STRING_HUFFMAN 12

class PacketController
{
public:

	static void read();
    static void write(int msg);

private:
    static void readFewInt();
    static void readFewString();
    static void readManyInt();
    static void readManyString();
    
    static void readFewIntLz();
    static void readFewStringLz();
    static void readManyIntLz();
    static void readManyStringLz();
    
    static void readFewIntHuffman();
    static void readFewStringHuffman();
    static void readManyIntHuffman();
    static void readManyStringHuffman();
    
    static void writeFewInt();
    static void writeFewString();
    static void writeManyInt();
    static void writeManyString();
    
    static void writeFewIntLz();
    static void writeFewStringLz();
    static void writeManyIntLz();
    static void writeManyStringLz();
    
    static void writeFewIntHuffman();
    static void writeFewStringHuffman();
    static void writeManyIntHuffman();
    static void writeManyStringHuffman();
};

#endif // __PACKET_CONTROLLER_H__
