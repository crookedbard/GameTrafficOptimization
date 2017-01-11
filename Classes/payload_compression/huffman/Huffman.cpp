/* 
 * File:   main.cpp
 * Author: chachalaca
 *
 * Created on 21. červen 2014, 16:30
 */

#include <iostream>
#include <fstream>
#include <algorithm>
#include <vector>
#include <bitset>
#include <limits> 
#include "HNode.h"
//#include "HNode.cpp"
#include "Huffman.h"
//#include "Huffman.cpp"

using namespace std;

/*
 * 
 */
int performHuffmanTest(int argc, char** argv) {
        
    std::vector<char> text;
        
    char str[256];

    std::cout << "Zadejte jmeno existujiciho textoveho souboru k zakodovani: ";
    std::cin.get (str,256);    
    
    std::ifstream is(str);     

    while (is.good())          
    {
      char c = is.get();       
      if (is.good())
          text.push_back(c);
    }

    is.close(); 
    
    Huffman<char>* h = new Huffman<char>();  
    
    
    std::vector<bool> code = h->encode(&text);       
    
    
    short r = code.size()%8;
    long bsize = ((code.size()-r)/8);
    
    std::ofstream outfile ("out.huf",std::ofstream::binary);
    
    unsigned char byte[1];
    unsigned char firstByte[1];
    
    firstByte[0] = 0;
    if((r-(r%4))/4) firstByte[0] |= 1 << 2;
    if(((r%4)-(r%2))/2) firstByte[0] |= 1 << 1;
    if(r%2) firstByte[0] |= 1;    
    
    outfile.write (reinterpret_cast<char *>(firstByte),1);
   
    for(int y=0;y<(bsize+1);y++) { //byte
        byte[0] = 0;
        for (int i=0; i < 8; ++i) { //bit
            if ((8*y+i) <= code.size() && code[8*y+i]) {
                byte[0] |= 1 << i;
            }                  
        }
        outfile.write (reinterpret_cast<char *>(byte),1);
    }
    
    
    outfile.close(); 
    
    std::vector<char> decodedText;
    decodedText = h->decode(&code);
    
    cout<<endl<<"Pro kontrolu:"<<endl;
    
    std::vector<char>::iterator ci = decodedText.begin();
    while(ci != decodedText.end()) {
        cout<< *ci;       
        ci++;       
    }

    return 0;
}

