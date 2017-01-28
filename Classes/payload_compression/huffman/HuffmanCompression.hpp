//
//  HuffmanCompression.hpp
//  GameTrafficOptimization
//
//  Created by Dainius Kreivys on 2017-01-13.
//
//

#ifndef HuffmanCompression_hpp
#define HuffmanCompression_hpp

#include <stdio.h>
#include <iostream>
#include <string>

class HuffmanCompression
{
public:
    static unsigned char * encode(std::string str, int &compSize);
	static std::string decode(unsigned char * sour, int csize);
    static bool performTests(float &cspeed, float &dspeed);
private:
    static unsigned char* performCompressionTest(float &cspeed, int &csize);
    static std::string performDecompressionTest(unsigned char* cstring, float &dspeed, int &csize);
};

#endif /* HuffmanCompression_hpp */
