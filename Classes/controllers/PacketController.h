#ifndef __PACKET_CONTROLLER_H__
#define __PACKET_CONTROLLER_H__

#define MSG_INTDATAFEW 1
#define MSG_STRINGDATAFEW 2
#define MSG_INTDATAMANY 3
#define MSG_STRINGDATAMANY 4

#define MSG_INTDATAFEW_LZ 5
#define MSG_STRINGDATAFEW_LZ 6
#define MSG_INTDATAMANY_LZ 7
#define MSG_STRINGDATAMANY_LZ 8

#define MSG_INTDATAFEW_HUFFMAN 9
#define MSG_STRINGDATAFEW_HUFFMAN 10
#define MSG_INTDATAMANY_HUFFMAN 11
#define MSG_STRINGDATAMANY_HUFFMAN 12

class PacketController
{
public:

	static void readIntDataFew();
	static void readStringDataFew();
	static void sendIntDataFew();
	static void sendStringDataFew();
	static void handle();
    //writeToBuffer
    //readFromBuffer
};

#endif // __PACKET_CONTROLLER_H__
