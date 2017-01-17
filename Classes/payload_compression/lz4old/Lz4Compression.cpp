//
//  Lz4Compression.cpp
//  GameTrafficOptimization
//
//  Created by Dainius Kreivys on 2017-01-15.
//
//

#include "Lz4Compression.hpp"
#define _CRT_SECURE_NO_WARNINGS

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
#include "Classes\payload_compression\smallz4\smallz4.h"
#else
#include "smallz4.h"
#endif

#include <stdio.h>    // stdin/stdout/stderr, fopen, ...
#ifdef _WIN32
#include <io.h>     // isatty()
#else
#include <unistd.h> // isatty()
#define _fileno fileno
#define _isatty isatty
#endif
#include <iostream>
#include <string>
#include <sstream>

/// error handler
static void error(const char* msg)
{
    fprintf(stderr, "ERROR: %s\n", msg);
    exit(1);
}

char * bufferIn;
const unsigned int maxChainLength = 65536; // "unlimited" because search window contains only 2^16 bytes
size_t getBytesFromIn(void* data, size_t numBytes)
{
    if (data && numBytes > 0){
		numBytes = std::strlen(bufferIn);

		data = new unsigned char[numBytes];
		strcpy((char*)data, bufferIn);
        
        //return fread(data, 1, numBytes, in);
        return sizeof(data);
    }
    
    return 0;
}
std::stringstream bufferOut;
void sendBytesToOut(const void* data, size_t numBytes)
{
    if (data && numBytes > 0)
        bufferOut << (char *)data;//fwrite(data, 1, numBytes, out);
}

/// read a single byte (with simple buffering)
static unsigned char getByteFromIn()
{
    // modify buffer size as you like ... for most use cases, bigger buffer aren't faster anymore - and even reducing to 1 byte works !
#define READ_BUFFER_SIZE 4*1024
    static unsigned char readBuffer[READ_BUFFER_SIZE];
    static unsigned int  pos       = 0;
    static unsigned int  available = 0;
    
    // refill buffer
    if (pos == available)
    {
        pos = 0;
//        available = fread(readBuffer, 1, READ_BUFFER_SIZE, in);
        if (available == 0)
            error("out of data");
    }
    
    // return a byte
    return readBuffer[pos++];
}
const char* Lz4Compression::encode(char* buffer)
{
//    size_t (*f1)(void* data, size_t numBytes);
//    f1 = &getBytesFromIn;
//    //*f1(buffer, std::strlen(buffer));
//    f1(buffer, std::strlen(buffer));
    bufferIn = buffer;
    smallz4::lz4(getBytesFromIn, sendBytesToOut, maxChainLength);
    
//    printf("%d %d",(int)testsize,(int)testsize2);
//        printf("\nEXAMPLE 2: %d BYTES OF DATA COMPRESSED TO %d BYTES - COMPRESSION RATE: %.2f%%\n", (int)encoder.getTotalByteRead(), (int)encoder.getTotalByteWritten(), (float)encoder.getTotalByteWritten() * 100.0 / encoder.getTotalByteRead());
    auto ret =bufferOut.str();
    return ret.c_str();
}

char* Lz4Compression::decode(char* buffer)
{
//    size_t (*f1)(void* data, size_t numBytes);
//    f1 = &getBytesFromIn;
//    f1(buffer, std::strlen(buffer));
//    smallz4::unlz4(getBytesFromIn, sendBytesToOut);
    
//    printf("\nEXAMPLE 2: %d BYTES OF DATA COMPRESSED TO %d BYTES - DECOMPRESSION RATE: %.2f%%\n", (int)decoder.getTotalByteRead(), (int)decoder.getTotalByteWritten(), (float)decoder.getTotalByteWritten() * 100.0 / decoder.getTotalByteRead());
//    
	return nullptr;
}


//std::pair<EncodingResults, DecodingResults> Lz4Compression::performTest()
//{
//    EncodingResults encodingResults;
//    DecodingResults decodingResults;
//    return std::make_pair(encodingResults,decodingResults);
//}
