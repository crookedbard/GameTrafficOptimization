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
	auto string1 = readstring();
	auto string2 = readstring();

    g_screenLog->log( LL_DEBUG, "%s", string1);
    g_screenLog->log( LL_DEBUG, "%s", string2);
	//auto chars1 = readchars();
	//auto chars2 = readchars();
}

void PacketController::readManyInt()
{
    
}

void PacketController::readManyString()
{
	for (int i = 0; i < 15; i++) {
		auto str = readstring();
        g_screenLog->log( LL_DEBUG, "%s", str);
	}
}

void PacketController::readFewIntLz()
{
    
}

void PacketController::readFewStringLz()
{
	auto encoded = readstring();
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

	std::string encoded = readstring();
	auto decoded = Lz4Compression::decode(encoded); //it doesn't decode properly...
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
	for (int i = 0; i < csize; i++)
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
    writeint(123);
    writeint(456);
}

void PacketController::writeFewString()
{
	clearbuffer();
	writebyte(MSG_FEW_STRING);
	writestring("DAINIUS 01");
	writestring("KREIVYS 10");
}

void PacketController::writeManyInt()
{
    
}

void PacketController::writeManyString()
{
	clearbuffer();
	writebyte(MSG_MANY_STRING);
	for (auto i = 0; i < 20; i++) {
		writestring("DAINIUS 01 ABCDEF HELLO HELLO BYE");
		writestring("KREIVYS 10 FGE FGE THANK YOU THANK YOU");
		writestring("Testas 23 BUYING Selling Looking for party");
	}
}

void PacketController::writeFewIntLz()
{
    
}

void PacketController::writeFewStringLz()
{
	std::string str = "DAINIUS 01";
	str += "KREIVYS 10";

	int csize;
	auto encoded = Lz4Compression::encode(str);
	clearbuffer();
	writebyte(MSG_FEW_STRING_LZ);
	writestring(encoded);
}

void PacketController::writeManyIntLz()
{
    
}

void PacketController::writeManyStringLz()
{
	std::string str = "";
	for (auto i = 0; i < 20; i++) {
		str += "DAINIUS 01 ABCDEF HELLO HELLO BYE";
		str += "KREIVYS 10 FGE FGE THANK YOU THANK YOU";
		str += "Testas 23 BUYING Selling Looking for party";
	}

	
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

	writestring(encoded);
}

void PacketController::writeFewIntHuffman()
{
    /*clearbuffer();
    writeint(123);
    writeint(456);
    auto buffer = getbuffer();
    auto encoded = HuffmanCompression::encode(buffer->data);
    clearbuffer();
    writebyte(MSG_FEW_INT);
    writechars("");//huffman tree
    writechars("");//huffman tree
	*/
}

void PacketController::writeFewStringHuffman()
{
	/*clearbuffer();
	//writestring("DAINIUS 01"); //tik viena string aptinka huffmanas
	//writestring("KREIVYS 10");
	writechars("DAINIUS 01");
	writechars("KREIVYS 10"); //kazkodel dar 10 gale prisideda
	auto buffer = getbuffer();*/

	std::string str = "DAINIUS 01" ;
	str += "KREIVYS 10";

	int csize;
	auto encoded = HuffmanCompression::encode(str/*buffer->data*/, csize);
	clearbuffer();
	writebyte(MSG_FEW_STRING_HUFFMAN);
	writeint(csize);
	for (int i = 0; i < csize; i++)
	{
		writebyte(encoded[i]);
	}

}

void PacketController::writeManyIntHuffman()
{
    
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
	for (auto i = 0; i < 20; i++) {
		str += "DAINIUS 01 ABCDEF HELLO HELLO BYE";
		str += "KREIVYS 10 FGE FGE THANK YOU THANK YOU";
		str += "Testas 23 BUYING Selling Looking for party";
	}

	int csize;
	auto encoded = HuffmanCompression::encode(str/*buffer->data*/, csize);
	clearbuffer();
	writebyte(MSG_MANY_STRING_HUFFMAN);
	writeint(csize);
	for (int i = 0; i < csize; i++)
	{
		writebyte(encoded[i]);
	}
}
