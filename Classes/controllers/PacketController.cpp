#include "PacketController.h"
#include "transport_protocols\tcp\39dll.h"


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