//
//  RohcCompression.hpp
//  GameTrafficOptimization
//
//  Created by Dainius Kreivys on 2017-01-15.
//
//
#ifndef RohcCompression_hpp
#define RohcCompression_hpp

#include <stdio.h>
//#include <iostream>
//#include <string>

class RohcCompression
{
public:
    static bool compress(char* payload);
    static bool decompress(char* payload);
};

#endif /* Lz4Compression_hpp */
