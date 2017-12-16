//
//  SimdCompression.hpp
//  GameTrafficOptimization
//
//  Created by Dainius Kreivys on 2017-01-15.
//
//
#ifndef SimdCompression_hpp
#define SimdCompression_hpp

#include <stdio.h>
#include <iostream>
#include <string>

class SimdCompression
{
public:
    static std::string encode(std::string buffer);
    static std::string decode(std::string buffer);
    static void test();
};

#endif /* SimdCompression_hpp */
