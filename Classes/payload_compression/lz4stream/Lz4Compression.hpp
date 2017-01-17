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

//struct EncodingResults {
//    int cyclesMin;
//    int cyclesMax;
//    int cyclesAverage;
//    int cyclesTotal;
//} ;
//
//struct DecodingResults {
//    int cyclesMin;
//    int cyclesMax;
//    int cyclesAverage;
//    int cyclesTotal;
//} ;
#include <iostream>
#include <string>

class Lz4Compression
{
public:
    static std::string encode(std::string buffer);
    static std::string decode(std::string buffer/*const char* buffer*/);
//    static std::pair<EncodingResults, DecodingResults> performTest();
};

#endif /* Lz4Compression_hpp */
