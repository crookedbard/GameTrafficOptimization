/*
 * lz4xx - C++ wrapper for LZ4 compression algorithm
 *
 * Author: Tsz-Ho Yu (tszhoyu@gmail.com)
 *
 */
#include "lz4.h"

#include <cstdio>
#include <cstdlib>
#include <cstring>

// A unified output stream for file/byte array.
class LZ4OutStream
{
    public:
        LZ4OutStream();
        // open destination buffer
        void open(char** inbuf, size_t* sizeloc);
        // open destination file
        void open(FILE* fd_in);
        // write to buffer/file
        size_t write(const void* in, const size_t inlen);
    protected:
        void** mBuf;
        size_t* mSize;
        size_t mSpace;
        int mIsOpened;
};

class LZ4InStream
{
    public:
        LZ4InStream();
        // open source buffer
        void open(const void* ptr, size_t inlen);
        // open source file
        void open(FILE* fd);
        // read from source file/buffer
        size_t read(void* ptr, size_t size);
    protected:
        size_t mSpace;
        char* mBuf;
        char* mReadPtr;
        int mIsOpened;
};

class LZ4Encoder
{
    public:
        LZ4Encoder();
        LZ4Encoder(const size_t blockSize); // customized block size
        ~LZ4Encoder();

        // open input file
        int open(FILE* out);

        // open input byte array
        int open(char** out, size_t* outlen);

        // encode to file
        size_t encode(FILE* in);

        // encode to byte array
        size_t encode(char* in, size_t inlen);

        // close the encoding pipe
        size_t close();

        // get total byte read and written
        size_t getTotalByteRead() const;
        size_t getTotalByteWritten() const;

    protected:
        size_t mBlockSize;          // block size
        size_t mBlockSpace;         // availalble space in block
        char*  mBlock;              // I/O block

        size_t encode(size_t inlen);
        size_t flush();              // internal - write block to file
        void init(const size_t blockSize = 8192);

        LZ4_stream_t mStream;       // stream
        LZ4OutStream mOut;          // output stream
        LZ4InStream mIn;            // input stream

        size_t mTotalRead;
        size_t mTotalWritten;
};

class LZ4Decoder
{
    public:
        LZ4Decoder();
        LZ4Decoder(const size_t blockSize); // customized blocksize
        ~LZ4Decoder();

        // open input file
        int open(FILE* out);

        // open input byte array
        int open(char** out, size_t* outlen);

        // decode to file
        size_t decode(FILE* in);

        // decode to byte array
        size_t decode(char* in, size_t inlen);

        // get total byte read and written
        size_t getTotalByteRead() const;
        size_t getTotalByteWritten() const;

    protected:
        size_t mBlockSize;                  // block size

        void init(const size_t blockSize = 8192);
        size_t decode(size_t inlen);

        LZ4InStream mIn;
        LZ4OutStream mOut;
        LZ4_streamDecode_t mStreamDecode;   // decode stream

        size_t mTotalRead;
        size_t mTotalWritten;
};
