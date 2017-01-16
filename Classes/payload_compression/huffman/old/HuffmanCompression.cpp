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
#include "HNode.h"
#include "Huffman.h"

const char* HuffmanCompression::encode(char* buffer)
{
    std::vector<char> text;
    for (int i = 0; i < strlen(buffer); i++){
        
        text.push_back(buffer[i]);
    }
    
    Huffman<char>* h = new Huffman<char>();
    std::vector<bool> code = h->encode(&text);
    
    
    short r = code.size()%8;
    long bsize = ((code.size()-r)/8);
    
    std::stringstream outbuff;
    
//    unsigned char byte[1];
//    unsigned char firstByte[1];
    
//    firstByte[0] = 0;
//    if((r-(r%4))/4) firstByte[0] |= 1 << 2;
//    if(((r%4)-(r%2))/2) firstByte[0] |= 1 << 1;
//    if(r%2) firstByte[0] |= 1;
//    
//    outbuff.write (reinterpret_cast<char *>(firstByte),1);
//    
//    for(int y=0;y<(bsize+1);y++) { //byte
//        byte[0] = 0;
//        for (int i=0; i < 8; ++i) { //bit
//            if ((8*y+i) <= code.size() && code[8*y+i]) {
//                byte[0] |= 1 << i;
//            }
//        }
//        outbuff.write (reinterpret_cast<char *>(byte),1);
//    }
//    
//    const std::string tmp = outbuff.str();
//    const char* cstr = tmp.c_str();
    
    //unsigned long codeSize=code.size();
    
    //unsigned long *codeSize;
    //codeSize = (unsigned long *)malloc(code.size());
    
    //std::bitset<const_cast<const unsigned long&>(codeSize)> b("asdasdasd");
    
    std::vector<std::bitset<8>> bytes;
    std::string bits;
    for(int y=0;y<=bsize;y++) { //byte
        bits ="";
        for (int i=0; i < 8; ++i) { //bit
            if ((8*y+i) < code.size() ) {
                if(code[8*y+i]) bits+='1';
                else bits+='0';
            }else break;
        }
        std::bitset<8> byte(bits);
        bytes.push_back(byte);
    }
    
    std::string encodedStr = "";
    for(auto b : bytes)
    {
        encodedStr += char(b.to_ulong());
    }
    
    //return encodedStr.c_str();
    
    //string to char bits
    std::vector<bool> newCode;
    for(char c : encodedStr)
    {
        std::bitset<8> ch(c);
        std::string bb = ch.to_string();
        for(char bbb : bb)
        {
            if(bbb  == '0')newCode.push_back(false);
            else newCode.push_back(true);
        }
    }
    
    std::vector<char> decodedText;
    //Huffman<char>* h = new Huffman<char>();
    decodedText = h->decode(&newCode);
    
    std::string fullstr = "";
    for(auto c : decodedText)
    {
        fullstr += c;
    }
    return fullstr.c_str();
    
}



const char* HuffmanCompression::decode(const char* buffer)
{
    std::string encodedStr(buffer);
    //string to char bits
    std::vector<bool> newCode;
    for(char c : encodedStr)
    {
        std::bitset<8> ch(c);
        std::string bb = ch.to_string();
        for(char bbb : bb)
        {
            if(bbb  == '0')newCode.push_back(false);
            else newCode.push_back(true);
        }
    }
    
    std::vector<char> decodedText;
    Huffman<char>* h = new Huffman<char>();
    decodedText = h->decode(&newCode);
    
    std::string fullstr = "";
    for(auto c : decodedText)
    {
        fullstr += c;
    }
    return fullstr.c_str();
}

std::pair<EncodingResults, DecodingResults> HuffmanCompression::performTest()
{
    EncodingResults encodingResults;
    DecodingResults decodingResults;
    return std::make_pair(encodingResults,decodingResults);
}
