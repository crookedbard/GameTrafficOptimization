#include <cstdio>
#include <cstring>
#include <cstdlib>

#include "lz4xx.h"

char test_data[] = "Quick brown fox jumps over the lazy dog\n";

int example2()
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
