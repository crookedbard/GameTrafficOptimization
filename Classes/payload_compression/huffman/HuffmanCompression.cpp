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

/*std::pair<EncodingResults, DecodingResults> HuffmanCompression::performTest()
{
    EncodingResults encodingResults;
    DecodingResults decodingResults;
    return std::make_pair(encodingResults,decodingResults);
}
*/