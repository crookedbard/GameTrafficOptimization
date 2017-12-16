#include "PacketController.h"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
#include "transport_protocols\tcp\39dll.h"
#include "payload_compression\huffman\HuffmanCompression.hpp"
#include "payload_compression\lz4stream\Lz4Compression.hpp"
#include "utils/ScreenLog.hpp"
#else
#include "39dll.h"
#include "HuffmanCompression.hpp"
#include "Lz4Compression.hpp"
#include "ScreenLog.hpp"
#endif

const char *SHORT_STRING_VALUE =
"Random mmorpg chat message\
Buying weapon\
Searching";

const char *LONG_STRING_VALUE =
"Random mmorpg chat dialogue, \
Looking for Party > Endless tower floor 3, \
LFP > Hotsprings LV70+, \
LFP > Forest of mirror 50lvl+ 3, \
RAID > Endless tower floor 10, \
Farming at Alligator island, \
Trading weapon for armor set, \
Buying augmentet weapon S grade, \
Selling Blue wolf gloves, boots";

void PacketController::read()
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
		case MSG_FEW_INT:
			readFewInt();
			break;
			
		case MSG_FEW_STRING:
			readFewString();
			break;
			
		case MSG_MANY_INT:
			readManyInt();
			break;
			
		case MSG_MANY_STRING:
			readManyString();
			break;

		case MSG_FEW_INT_LZ:
			readFewIntLz();
			break;
			
		case MSG_FEW_STRING_LZ:
			readFewStringLz();
			break;
			
		case MSG_MANY_INT_LZ:
			readManyIntLz();
			break;
			
		case MSG_MANY_STRING_LZ:
			readManyStringLz();
			break;
			
		case MSG_FEW_INT_HUFFMAN:
			readFewIntHuffman();
			break;
			
		case MSG_FEW_STRING_HUFFMAN:
			readFewStringHuffman();
			break;
			
		case MSG_MANY_INT_HUFFMAN:
			readManyIntHuffman();
			break;
			
		case MSG_MANY_STRING_HUFFMAN:
			readManyStringHuffman();
			break;
			
		default:
			break;
	}
}

void PacketController::write(int msg)
{
	switch (msg)
	{
		case MSG_FEW_INT:
			writeFewInt();
			break;
			
		case MSG_FEW_STRING:
			writeFewString();
			break;
			
		case MSG_MANY_INT:
			writeManyInt();
			break;
			
		case MSG_MANY_STRING:
			writeManyString();
			break;
			
		case MSG_FEW_INT_LZ:
			writeFewIntLz();
			break;
			
		case MSG_FEW_STRING_LZ:
			writeFewStringLz();
			break;
			
		case MSG_MANY_INT_LZ:
			writeManyIntLz();
			break;
			
		case MSG_MANY_STRING_LZ:
			writeManyStringLz();
			break;
			
		case MSG_FEW_INT_HUFFMAN:
			writeFewIntHuffman();
			break;
			
		case MSG_FEW_STRING_HUFFMAN:
			writeFewStringHuffman();
			break;
			
		case MSG_MANY_INT_HUFFMAN:
			writeManyIntHuffman();
			break;
			
		case MSG_MANY_STRING_HUFFMAN:
			writeManyStringHuffman();
			break;
			
		default:
			break;
	}
}

void PacketController::readFewInt()
{
	
}

void PacketController::readFewString()
{
//	auto string1 = readstring();
//	auto string2 = readstring();
//
//	g_screenLog->log( LL_DEBUG, "%s", string1);
//	g_screenLog->log( LL_DEBUG, "%s", string2);
	//auto chars1 = readchars();
	//auto chars2 = readchars();

	auto str = readstring();
	g_screenLog->log(LL_DEBUG, "%s", str);
}

void PacketController::readManyInt()
{
	
}

void PacketController::readManyString()
{
//	for (int i = 0; i < 15; i++) {
//		auto str = readstring();
//		g_screenLog->log( LL_DEBUG, "%s", str);
//	}

	auto str = readstring();
	g_screenLog->log(LL_DEBUG, "%s", str);
}

void PacketController::readFewIntLz()
{
	
}

void PacketController::readFewStringLz()
{
//	auto encoded = readstring();
	auto csize = readint();
	auto encoded = new char[csize];
	for (auto i = 0; i < csize; i++)
	{
		encoded[i] = readbyte();
	}

	auto decoded = Lz4Compression::decode(encoded);
	g_screenLog->log( LL_DEBUG, "%s", decoded.c_str());
}

void PacketController::readManyIntLz()
{
	
}

void PacketController::readManyStringLz()
{
	//auto size = readint();
	//auto encoded = readchars(size); //doesn't work
	auto csize = readint();
	auto encoded = new unsigned char[csize];
	for (auto i = 0; i < csize; i++)
	{
		encoded[i] = readbyte();
	}
//	std::string encoded = readstring();
	auto decoded = Lz4Compression::decode(reinterpret_cast<char*>(encoded)); //it doesn't decode properly...
	g_screenLog->log( LL_DEBUG, "%s", decoded.c_str());
	//also doesn't work:
	/*auto csize = readint();
	auto encoded = new char[csize];
	for (int i = 0; i < csize; i++)
	{
		encoded[i] = readbyte();
	}
	std::string s(encoded);
	auto decoded = Lz4Compression::decode(s);*/
}

void PacketController::readFewIntHuffman()
{
	
}

void PacketController::readFewStringHuffman()
{
	auto csize = readint();
	auto encoded = new unsigned char[csize];
	for (auto i = 0; i < csize; i++)
	{
		encoded[i] = readbyte();
	}
	auto decoded = HuffmanCompression::decode(encoded, csize);
	g_screenLog->log( LL_DEBUG, "%s", decoded.c_str());
}

void PacketController::readManyIntHuffman()
{
	
}

void PacketController::readManyStringHuffman()
{
	auto csize = readint();
	auto encoded = new unsigned char[csize];
	for (int i = 0; i < csize; i++)
	{
		encoded[i] = readbyte();
	}
	auto decoded = HuffmanCompression::decode(encoded, csize);
	g_screenLog->log( LL_DEBUG, "%s", decoded.c_str());
}

void PacketController::writeFewInt()
{
	clearbuffer();
	writebyte(MSG_FEW_INT);
    writeint(8);
	writeint(123);
	writeint(456);
    writeint(123);
    writebyte(1);
    writebyte(1);
    writebyte(1);
}

void PacketController::writeFewString()
{
	clearbuffer();
	writebyte(MSG_FEW_STRING);
//	writestring("DAINIUS 01");
//	writestring("KREIVYS 10");
	writestring(SHORT_STRING_VALUE);
}

void PacketController::writeManyInt()
{
    clearbuffer();
    writebyte(MSG_FEW_INT);
    writeint(8);
    writeint(123);
    writeint(456);
    writeint(8);
    writeint(123);
    writeint(456);
    writeint(8);
    writeint(123);
    writeint(456);
    writeint(8);
    writeint(123);
    writeint(456);
    writeint(8);
    writeint(123);
    writeint(456);
    writeint(8);
    writeint(123);
    writeint(456);
    writeint(8);
    writeint(123);
    writeint(456);
    writeint(8);
    writeint(123);
    writeint(456);
    writebyte(1);
    writebyte(1);
    writebyte(1);
}

void PacketController::writeManyString()
{
	clearbuffer();
	writebyte(MSG_MANY_STRING);
//	std::string str = LONG_STRING_VALUE;
//	str += LONG_STRING_VALUE;
//	str += LONG_STRING_VALUE;
//	str += LONG_STRING_VALUE;
//
//
    std::string str = "";
    for (auto i = 0; i < 10; i++) {
        str += "Looking for Party> Endless tower floor 3";
        str += "Buying augmentet weapon grade";
        str += "Selling Blue wolf gloves, boots";
    }
    str.pop_back();str.pop_back();
    writestring(str);
}

void PacketController::writeFewIntLz()
{
    clearbuffer();
    writebyte(1);
    writeint(8);
    writeint(123);
    writeint(456);
    writeint(123);
    writebyte(1);
    writebyte(1);
    writebyte(1);
    auto buffer = getbuffer();
    auto encoded = Lz4Compression::encode(buffer->data);
    clearbuffer();
    writebyte(MSG_FEW_INT_LZ);
    writeint(encoded.length());
    
    auto sour = new unsigned char[encoded.length()];
    strcpy((char*)sour, encoded.c_str());
    
    for (auto i = 0; i < encoded.length(); i++)
    {
        writebyte(sour[i]);
    }
}

void PacketController::writeFewStringLz()
{
//	std::string str = "DAINIUS 01";
//	str += "KREIVYS 10";

	//int csize;
	auto encoded = Lz4Compression::encode(SHORT_STRING_VALUE);
	clearbuffer();
	writebyte(MSG_FEW_STRING_LZ);
    writeint(encoded.length());
    
    auto sour = new unsigned char[encoded.length()];
    strcpy((char*)sour, encoded.c_str());
    
    for (auto i = 0; i < encoded.length(); i++)
    {
        writebyte(sour[i]);
    }
}

void PacketController::writeManyIntLz()
{
    clearbuffer();
    writebyte(1);
    writeint(8);
    writeint(123);
    writeint(456);
    writeint(8);
    writeint(123);
    writeint(456);
    writeint(8);
    writeint(123);
    writeint(456);
    writeint(8);
    writeint(123);
    writeint(456);
    writeint(8);
    writeint(123);
    writeint(456);
    writeint(8);
    writeint(123);
    writeint(456);
    writeint(8);
    writeint(123);
    writeint(456);
    writeint(8);
    writeint(123);
    writeint(456);
    writebyte(1);
    writebyte(1);
    writebyte(1);
    auto buffer = getbuffer();
    auto encoded = Lz4Compression::encode(buffer->data);
    clearbuffer();
    writebyte(MSG_FEW_INT_LZ);
    writeint(encoded.length());
    
    auto sour = new unsigned char[encoded.length()];
    strcpy((char*)sour, encoded.c_str());
    
    for (auto i = 0; i < encoded.length(); i++)
    {
        writebyte(sour[i]);
    }
}

void PacketController::writeManyStringLz()
{
	std::string str = "";
	for (auto i = 0; i < 10; i++) {
		str += "Looking for Party > Endless tower floor 3";
		str += "Buying augmentet weapon S grade";
		str += "Selling Blue wolf gloves, boots";
	}
str.pop_back();str.pop_back();
//	std::string str = LONG_STRING_VALUE;
//	str += LONG_STRING_VALUE;
//	str += LONG_STRING_VALUE;
//	str += LONG_STRING_VALUE;

	//int csize;
	auto encoded = Lz4Compression::encode(str);
	clearbuffer();
	writebyte(MSG_MANY_STRING_LZ);

	/*int csize = encoded.length();
	writeint(csize);
	for (int i = 0; i < csize; i++)
	{
		writebyte(encoded[i]);
	}*/

	//writeint(encoded.length());
	//writechars((char *)encoded.c_str());

	writeint(encoded.length());

	auto sour = new unsigned char[encoded.length()];
	strcpy((char*)sour, encoded.c_str());

	for (auto i = 0; i < encoded.length(); i++)
	{
		writebyte(sour[i]);
	}

	//writestring(encoded);
}

void PacketController::writeFewIntHuffman()
{
	clearbuffer();
    writebyte(1);
    writeint(8);
    writeint(123);
    writeint(456);
    writeint(123);
    writebyte(1);
    writebyte(1);
    writebyte(1);
	auto buffer = getbuffer();
    int csize;
	auto encoded = HuffmanCompression::encode(buffer->data, csize);
	clearbuffer();
	writebyte(MSG_FEW_INT_HUFFMAN);
    writeint(csize);
    for (auto i = 0; i < csize; i++)
    {
        writebyte(encoded[i]);
    }
}

void PacketController::writeFewStringHuffman()
{
	/*clearbuffer();
	//writestring("DAINIUS 01"); //tik viena string aptinka huffmanas
	//writestring("KREIVYS 10");
	writechars("DAINIUS 01");
	writechars("KREIVYS 10"); //kazkodel dar 10 gale prisideda
	auto buffer = getbuffer();*/

//	std::string str = "Buying weapon" ;
//	str += "Selling armor";

	int csize;
	auto encoded = HuffmanCompression::encode(SHORT_STRING_VALUE/*buffer->data*/, csize);
	clearbuffer();
	writebyte(MSG_FEW_STRING_HUFFMAN);
	writeint(csize);
	for (auto i = 0; i < csize; i++)
	{
		writebyte(encoded[i]);
	}

}

void PacketController::writeManyIntHuffman()
{
    clearbuffer();
    writebyte(1);
    writeint(8);
    writeint(123);
    writeint(456);
    writeint(8);
    writeint(123);
    writeint(456);
    writeint(8);
    writeint(123);
    writeint(456);
    writeint(8);
    writeint(123);
    writeint(456);
    writeint(8);
    writeint(123);
    writeint(456);
    writeint(8);
    writeint(123);
    writeint(456);
    writeint(8);
    writeint(123);
    writeint(456);
    writeint(8);
    writeint(123);
    writeint(456);
    writebyte(1);
    writebyte(1);
    writebyte(1);
    auto buffer = getbuffer();
    int csize;
    auto encoded = HuffmanCompression::encode(buffer->data, csize);
    clearbuffer();
    writebyte(MSG_FEW_INT_HUFFMAN);
    writeint(csize);
    for (auto i = 0; i < csize; i++)
    {
        writebyte(encoded[i]);
    }
}

void PacketController::writeManyStringHuffman()
{
	/*clearbuffer();
	for (auto i = 0; i < 5; i++) {
		writechars("DAINIUS 01 ABCDEF HELLO HELLO BYE");
		writechars("KREIVYS 10 FGE FGE THANK YOU THANK YOU");
		writechars("Testas 23 BUYING Selling Looking for party");
	}
	auto buffer = getbuffer();*/

	std::string str = "";
	for (auto i = 0; i < 10; i++) {
		str += "Looking for Party > Endless tower floor 3";
		str += "Buying augmentet weapon S grade";
		str += "Selling Blue wolf gloves, boots";
	}
    str.pop_back();str.pop_back();
//	std::string str = LONG_STRING_VALUE;
//	str += LONG_STRING_VALUE;
//	str += LONG_STRING_VALUE;
//	str += LONG_STRING_VALUE;
	int csize;
	auto encoded = HuffmanCompression::encode(str/*buffer->data*/, csize);
	clearbuffer();
	writebyte(MSG_MANY_STRING_HUFFMAN);
	writeint(csize);
	for (auto i = 0; i < csize; i++)
	{
		writebyte(encoded[i]);
	}
}
