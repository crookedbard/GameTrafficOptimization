#include <cstdio>
#include <cstring>
#include <cstdlib>

#include "lz4xx.h"

int main(int argc, char* argv[])
{

    // check the number of arguments
    if (argc < 2)
    {
        printf("USAGE: %s [test file]\n", argv[0]);
        return 0;
    }

    // set up encoder and decoder
    size_t BLOCKSIZE = 8192;

    // initialize encoder and decoder
    LZ4Encoder encoder(BLOCKSIZE);
    LZ4Decoder decoder(BLOCKSIZE);

    // file descriptor
    FILE* fd_in;
    FILE* fd_out;

    // setup file names
    char inPath[256];
    char lzPath[256];
    char outPath[256];
    sprintf(inPath, "%s", argv[1]);
    sprintf(lzPath, "%s.lz4", argv[1]);
    sprintf(outPath, "%s.lz4.out", argv[1]);

    printf("\nEXAMPLE 1: \n(1) COMPRESS %s TO %s\n(2) DECOMPRESS %s TO %s\n", inPath, lzPath, lzPath, outPath);

    // Encode a file to file
    fd_in = fopen(inPath, "r");
    fd_out = fopen(lzPath, "w");
    encoder.open(fd_out);
    encoder.encode(fd_in);
    encoder.close();
    fclose(fd_in);
    fclose(fd_out);

    // Decode file to file
    fd_in = fopen(lzPath, "r");
    fd_out = fopen(outPath, "w");
    decoder.open(fd_in);
    decoder.decode(fd_out);
    fclose(fd_in);
    fclose(fd_out);

    printf("\nEXAMPLE 1: %d BYTES OF DATA COMPRESSED TO %d BYTES - COMPRESSION RATE: %.2f%%\n", (int)encoder.getTotalByteRead(), (int)encoder.getTotalByteWritten(), (float)encoder.getTotalByteWritten() * 100.0 / encoder.getTotalByteRead());

    printf("\nEXAMPLE 1: %d BYTES OF DATA COMPRESSED TO %d BYTES - DECOMPRESSION RATE: %.2f%%\n", (int)decoder.getTotalByteRead(), (int)decoder.getTotalByteWritten(), (float)decoder.getTotalByteWritten() * 100.0 / decoder.getTotalByteRead());

    return 0;
}
