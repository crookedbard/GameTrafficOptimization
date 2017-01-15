//
//  Lz4Compression.cpp
//  GameTrafficOptimization
//
//  Created by Dainius Kreivys on 2017-01-15.
//
//

#include "Lz4Compression.hpp"
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include "lz4xx.h"

//size_t BLOCKSIZE = 8192;
//// initialize LZ4 encoder and decoder
//LZ4Encoder encoder(BLOCKSIZE);
//LZ4Decoder decoder(BLOCKSIZE);
//
//char* Lz4Compression::encode(char* buffer)
//{
//    // display results
//    printf("\nINPUT TEXT:\n%s\n", buffer);
//    // set up encoder and decoder
//    
//    
//    // compressed buffer
//    char* lzBuf;
//    size_t lzBufSize;
//    
//    
//    
//    
//    
//    lzBufSize = 0; // initialize buffer size
//    
//    // encode
//    char eos = '\0';
//    encoder.open(&lzBuf, &lzBufSize);
//    encoder.encode(buffer, strlen(buffer));
//    encoder.encode(&eos, sizeof(eos));
//    encoder.close();
//    auto testsize = sizeof(lzBuf);
//    auto testsize2 = strlen(lzBuf);
//    printf("%d %d",(int)testsize,(int)testsize2);
//        printf("\nEXAMPLE 2: %d BYTES OF DATA COMPRESSED TO %d BYTES - COMPRESSION RATE: %.2f%%\n", (int)encoder.getTotalByteRead(), (int)encoder.getTotalByteWritten(), (float)encoder.getTotalByteWritten() * 100.0 / encoder.getTotalByteRead());
//    
//    //free(buffer);
//    return lzBuf;
//}
//
//char* Lz4Compression::decode(char* buffer)
//{
//    // decomopressed buffer
//    char *outBuf;
//    size_t outSize;
//    
//    // dencode
//    decoder.open(&outBuf, &outSize);
//    decoder.decode(buffer, 4);//sizeof(buffer));
//    
//    
//    // display results
//    printf("\nOUTPUT TEXT:\n%s\n", outBuf);
//    
//
//    
//    printf("\nEXAMPLE 2: %d BYTES OF DATA COMPRESSED TO %d BYTES - DECOMPRESSION RATE: %.2f%%\n", (int)decoder.getTotalByteRead(), (int)decoder.getTotalByteWritten(), (float)decoder.getTotalByteWritten() * 100.0 / decoder.getTotalByteRead());
//    
//    //free(buffer);
//    return outBuf;
//}

char test_data[] = "Quick brown fox jumps over the lazy dog\n";

int Lz4Compression::exampleLz4()
{
    
    // set up encoder and decoder
    size_t BLOCKSIZE = 8192;
    
    // compressed buffer
    char* lzBuf;
    size_t lzBufSize;
    
    // decomopressed buffer
    char *outBuf;
    size_t outSize;
    
    // initialize LZ4 encoder and decoder
    LZ4Encoder encoder(BLOCKSIZE);
    LZ4Decoder decoder(BLOCKSIZE);
    
    lzBufSize = 0; // initialize buffer size
    
    printf("\nEXAMPLE 2: \n(1) COMPRESS 20 TEST DATA\n(2) DECOMPRESS THE TEST DATA. \n");
    
    // encode
    char eos = '\0';
    encoder.open(&lzBuf, &lzBufSize);
    for (int i = 0; i < 20; i++)
    {
        encoder.encode(test_data, strlen(test_data));
    }
    encoder.encode(&eos, sizeof(eos));
    encoder.close();
    
    // encode
    decoder.open(&outBuf, &outSize);
    decoder.decode(lzBuf, lzBufSize);
    
    // display results
    printf("\nINPUT TEXT:\n%s\n", test_data);
    
    printf("\nOUTPUT TEXT:\n%s\n", outBuf);
    
    printf("\nEXAMPLE 2: %d BYTES OF DATA COMPRESSED TO %d BYTES - COMPRESSION RATE: %.2f%%\n", (int)encoder.getTotalByteRead(), (int)encoder.getTotalByteWritten(), (float)encoder.getTotalByteWritten() * 100.0 / encoder.getTotalByteRead());
    
    printf("\nEXAMPLE 2: %d BYTES OF DATA COMPRESSED TO %d BYTES - DECOMPRESSION RATE: %.2f%%\n", (int)decoder.getTotalByteRead(), (int)decoder.getTotalByteWritten(), (float)decoder.getTotalByteWritten() * 100.0 / decoder.getTotalByteRead());
    
    free(lzBuf);
    
    return 0;
}

//std::pair<EncodingResults, DecodingResults> Lz4Compression::performTest()
//{
//    EncodingResults encodingResults;
//    DecodingResults decodingResults;
//    return std::make_pair(encodingResults,decodingResults);
//}
