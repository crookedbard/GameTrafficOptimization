#include "PacketController.h"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
#include "transport_protocols\tcp\39dll.h"
#include "payload_compression\huffman\HuffmanCompression.hpp"
#else
#include "39dll.h"
#include "HuffmanCompression.hpp"
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
    
}

void PacketController::readManyInt()
{
    
}

void PacketController::readManyString()
{
    
}

void PacketController::readFewIntLz()
{
    
}

void PacketController::readFewStringLz()
{
    
}

void PacketController::readManyIntLz()
{
    
}

void PacketController::readManyStringLz()
{
    
}

void PacketController::readFewIntHuffman()
{
    
}

void PacketController::readFewStringHuffman()
{
    
}

void PacketController::readManyIntHuffman()
{
    
}

void PacketController::readManyStringHuffman()
{
    
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
    
}

void PacketController::writeManyInt()
{
    
}

void PacketController::writeManyString()
{
    
}

void PacketController::writeFewIntLz()
{
    
}

void PacketController::writeFewStringLz()
{
    
}

void PacketController::writeManyIntLz()
{
    
}

void PacketController::writeManyStringLz()
{
    
}

void PacketController::writeFewIntHuffman()
{
    clearbuffer();
    writeint(123);
    writeint(456);
    auto buffer = getbuffer();
    auto encoded = HuffmanCompression::encode(buffer->data);
    clearbuffer();
    writebyte(MSG_FEW_INT);
    writechars("");//huffman tree
    writechars("");//huffman tree
}

void PacketController::writeFewStringHuffman()
{
    
}

void PacketController::writeManyIntHuffman()
{
    
}

void PacketController::writeManyStringHuffman()
{
    
}
