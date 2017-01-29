//
//  HuffmanCompression.cpp
//  GameTrafficOptimization
//
//  Created by Dainius Kreivys on 2017-01-13.
//
//

#include "HuffmanCompression.hpp"
#include <iostream>
#include <fstream>
#include <algorithm>
#include <vector>
#include <bitset>
#include <limits>
#include "Huffman.h"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
#include "utils/ScreenLog.hpp"
#include "transport_protocols/tcp/Common.h"
//#include "utils/Timer.h"
//#include "utils/Utils.h"
#else
#include "ScreenLog.hpp"
#include "Common.h"
//#include "Timer.h"

#endif

unsigned char * HuffmanCompression::encode(std::string str, int &compSize) 
{
	auto buffer = str.c_str();

	int usize, csize;
	usize = std::strlen(buffer);

	auto sour = new unsigned char[usize];
	strcpy((char*)sour, buffer);

	Huffman huffman;

	auto dest = new unsigned char[usize];
	huffman.encode(dest, csize, sour, usize);

	wprintf(L" cmpression ration: %.2f\n", (float)usize / (float)csize);
	compSize = csize;
	return dest;
}

void CharToByte(char* chars, unsigned char * bytes, unsigned int count) {
	for (unsigned int i = 0; i < count; i++)
		bytes[i] = (unsigned char)chars[i];
}

std::string HuffmanCompression::decode(unsigned char * sour, int csize) 
{
	Huffman huffman;

	int usize = huffman.get_uncompressed_size(sour);

	if (usize < csize) usize = csize;
	auto dest = new unsigned char[usize];

	huffman.decode(dest, usize, sour);

	std::string res = "";
	for (int i = 0; i < usize; i++) {
		res += dest[i];
	}

	return res;
}

bool HuffmanCompression::performTests(float &cspeed, float &dspeed)
{
	try
	{
		int csize;
		auto cstring = performCompressionTest(cspeed, csize);
		auto dstring = performDecompressionTest(cstring, dspeed, csize);
	}
	catch (const std::exception& e)
	{
		printf("%s", e.what());
		return false;
	}
	
	return true;
}

unsigned char* HuffmanCompression::performCompressionTest(float &cspeed,int &csize)
{
	const int MB_SIZE = 25;
	const int MB_BYTES = 1000000; //1 MB = 1000000 Bytes
	const int SEC_MILI = 1000; //1 second = 1000 milis
	
	const int CHUNK_SIZE = MB_SIZE * MB_BYTES;
	auto buffer = random_string(CHUNK_SIZE);
	printf("Buffer length = %.3f MB ", static_cast<float>(buffer.length()) / static_cast<float>(MB_BYTES));
	auto timestart = getTimeMillis();
	auto cstring = encode(buffer, csize);
	auto timeend = getTimeMillis();
	auto timeDif = timeend - timestart;
	printf("Compressed length = %.3f MB ", static_cast<float>(csize) / static_cast<float>(MB_BYTES));
	printf("Total compression time = %.3f ", timeDif / static_cast<float>(SEC_MILI));
	cspeed = static_cast<float>(SEC_MILI) * static_cast<float>(MB_SIZE) / timeDif;
	//cspeed = res;
	//printf("%f MB/s ", res);
	return cstring;
}

/**
 * \brief performs decompression test
 * \return - speed MB/s
 */
std::string HuffmanCompression::performDecompressionTest(unsigned char* cstring, float &dspeed, int &csize)
{
	const int MB_BYTES = 1000000; //1 MB = 1000000 Bytes
	const int SEC_MILI = 1000; //1 second = 1000 milis
	
	auto timestart = getTimeMillis();
	auto dstring = decode(cstring, csize);
	auto timeend = getTimeMillis();//Timer::GetTimeInMiliseconds();
	auto timeDif = timeend - timestart;
	printf("Decompressed length = %.3f MB ", static_cast<float>(dstring.length()) / static_cast<float>(MB_BYTES));
	printf("Total decompression time = %.3f ", timeDif / static_cast<float>(SEC_MILI));
	dspeed = static_cast<float>(SEC_MILI) * (static_cast<float>(dstring.length()) / static_cast<float>(MB_BYTES)) / timeDif;
	return dstring;
}
