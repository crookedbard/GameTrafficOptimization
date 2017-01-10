#include "PacketController.h"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
#include "transport_protocols\tcp\39dll.h"
#else
#include "39dll.h"
#endif


void PacketController::handle()
{
	//Handle incoming messages
	//  if (server::bufferEncrypt)
	//	{
	//Only decrypt if we're using an encryption password
	//		bufferencrypt(server::bufferPassword);
	//	}

	auto id = static_cast<int>(readbyte());

	switch (id)
	{
	case MSG_INTDATAFEW:
		readIntDataFew();
		break;

	case MSG_STRINGDATAFEW:
		readStringDataFew();
		break;

	default:
		//cout << "UNKNOWN PACKET:" << endl;
		break;
	}
}

void PacketController::readIntDataFew()
{

}

void PacketController::readStringDataFew()
{

}

void PacketController::sendIntDataFew()
{

}

void PacketController::sendStringDataFew()
{

}
