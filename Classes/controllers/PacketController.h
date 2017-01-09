#ifndef __PACKET_CONTROLLER_H__
#define __PACKET_CONTROLLER_H__

#define MSG_INTDATAFEW 1
#define MSG_STRINGDATAFEW 2
#define MSG_INTDATAMANY 2
#define MSG_STRINGDATAMANY 3

class PacketController
{
public:

	static void readIntDataFew();
	static void readStringDataFew();
	static void sendIntDataFew();
	static void sendStringDataFew();
	static void handle();
};

#endif // __PACKET_CONTROLLER_H__
