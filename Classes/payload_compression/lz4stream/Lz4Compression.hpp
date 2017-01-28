//
//  Lz4Compression.hpp
//  GameTrafficOptimization
//
//  Created by Dainius Kreivys on 2017-01-15.
//
//
#ifndef Lz4Compression_hpp
#define Lz4Compression_hpp

#include <stdio.h>
#include <iostream>
#include <string>

class Lz4Compression
{
public:
    static std::string encode(std::string buffer);//, int &size);
    static std::string decode(std::string buffer/*const char* buffer*/);
	static bool performTests(float &cspeed, float &dspeed);
private:
    static float performCompressionTest(std::string &cstring);
	static float performDecompressionTest(std::string &cstring);
	//static void meassure();
};

#endif /* Lz4Compression_hpp */
