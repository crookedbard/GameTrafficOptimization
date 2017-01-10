/*
 * lz4xx - C++ wrapper for LZ4 compression algorithm
 *
 * Author: Tsz-Ho Yu (tszhoyu@gmail.com)
 *
 */

#include "lz4xx.h"

static const size_t LZ4BLOCKSIZE = 8192;

LZ4OutStream::LZ4OutStream()
{
    mIsOpened = 0;
}

void LZ4OutStream::open(char** ptr, size_t* sizeloc)
{
    *sizeloc = 0;
    *ptr = NULL;
    mSize = sizeloc;
    mBuf = (void**)ptr;
    mSpace = 0;
    mIsOpened = 1;
}

void LZ4OutStream::open(FILE* fd)
{
    mBuf = (void**) fd;
    mSpace = 0;
    mSize = NULL;
    mIsOpened = 2;
}

size_t LZ4OutStream::write(const void* in, const size_t inlen)
{
    if (mIsOpened == 1)
    {
        if (*mSize + inlen > mSpace)
        {
            mSpace = ((*mSize) + inlen) << 1;
            *mBuf = (void*)realloc(*mBuf, mSpace);
            if (*mBuf == NULL)
            {
                return 0;
            }
        }
        memcpy((char*)*mBuf + *mSize, in, inlen);
        *mSize += inlen;
        return inlen;
    }
    if (mIsOpened == 2)
    {
        return fwrite(in, 1, inlen, (FILE*)mBuf);
    }
    return 0;
}


LZ4InStream::LZ4InStream()
{
    mIsOpened = 0;
}

void LZ4InStream::open(const void* ptr, size_t inlen)
{
    mSpace = inlen;
    mBuf = (char*) ptr;
    mReadPtr = mBuf;
    mIsOpened = 1;
}

void LZ4InStream::open(FILE *fd)
{
    mBuf = (char*) fd;
    mReadPtr = NULL;
    mIsOpened = 2;
}

size_t LZ4InStream::read(void* ptr, size_t size)
{
    if (mIsOpened == 1)
    {
        if (((mBuf + mSpace - mReadPtr) < size) && (mSpace > 0))
        {
            size = mBuf + mSpace - mReadPtr;
            if (size < 0)
            {
                return 0;
            }
        }
        memcpy(ptr, mReadPtr, size);
        mReadPtr += size;
        return size;
    }
    if (mIsOpened == 2)
    {
        return fread(ptr, 1, size, (FILE*) mBuf);
    }
    return 0;
}

LZ4Encoder::LZ4Encoder()
{
    init(LZ4BLOCKSIZE);
}

LZ4Encoder::LZ4Encoder(const size_t blockSize)
{
    init(blockSize);
}

void LZ4Encoder::init(const size_t blockSize)
{
    mBlockSize = blockSize;
    mBlock = NULL;
    mTotalWritten = 0;
    mTotalRead = 0;
    mBlock = new char[mBlockSize];
}

int LZ4Encoder::open(FILE* out)
{
    // set file
    mOut.open(out);
    // init LZ4
    LZ4_resetStream(&mStream);
    // init block size
    mBlockSpace = mBlockSize;
    return 0;
}

int LZ4Encoder::open(char** out, size_t* outlen)
{
    // init LZ4
    mOut.open(out, outlen);
    LZ4_resetStream(&mStream);
    // init block size
    mBlockSpace = mBlockSize;
    return 0;
}

size_t LZ4Encoder::encode(char* in, size_t inlen)
{
    mIn.open(in, inlen);
    return encode(inlen);
}

size_t LZ4Encoder::encode(FILE* in)
{
    mIn.open(in);
    return encode(-1);
}

size_t LZ4Encoder::encode(size_t inlen)
{
    size_t remain = inlen;
    while (1)
    {
        char* bufPtr = mBlock + mBlockSize - mBlockSpace;

        size_t toRead = (remain > mBlockSpace || inlen < 0) ? mBlockSpace : remain;

        size_t readSize = mIn.read(bufPtr, toRead);
        if (readSize == 0)
        {
            break;
        }
        remain -= readSize;
        mTotalRead += readSize;
        mBlockSpace -= readSize;
        if (remain <= 0)
        {
            break;
        }
        if (mBlockSpace == 0){
            mTotalWritten += flush();
        }
    }
    return mTotalWritten;
}

size_t LZ4Encoder::close()
{
    int zero = 0;
    mTotalWritten += flush();
    mTotalWritten += mOut.write(&zero, sizeof(zero));
    return mTotalWritten;
}

size_t LZ4Encoder::flush()
{
    // current block size
    size_t written = 0;
    size_t rawBlockSize = mBlockSize - mBlockSpace;
    //char cBlock[LZ4_COMPRESSBOUND(rawBlockSize)]; //expression must have a const size
	char *cBlock;
	cBlock = (char *)malloc(LZ4_COMPRESSBOUND(rawBlockSize));

    int cBlockSize = LZ4_compress_fast_continue(&mStream, mBlock, cBlock, rawBlockSize, sizeof(cBlock), 1);
    if (cBlockSize <= 0)
    {
        return -1; // if compression fails
    }
    // for debug use
    /*
    printf("%6d bytes -> %6d bytes, compression rate %10.2f%%\n", (int) rawBlockSize, (int) cBlockSize, (float)(100.0 * cBlockSize)/rawBlockSize);
    */
    // write to mOutPtr
    written += mOut.write(&cBlockSize, sizeof(cBlockSize));
    written += mOut.write(&cBlock, (size_t) cBlockSize);
    // reset block
    mBlockSpace = mBlockSize;
    return written;
}

size_t LZ4Encoder::getTotalByteWritten() const
{
    return mTotalWritten;
}

size_t LZ4Encoder::getTotalByteRead() const
{
    return mTotalRead;
}

LZ4Encoder::~LZ4Encoder()
{
    if (mBlock != NULL)
    {
        delete[] mBlock;
    }
}

LZ4Decoder::LZ4Decoder()
{
    init(LZ4BLOCKSIZE);
}

LZ4Decoder::LZ4Decoder(const size_t blockSize)
{
    init(blockSize);
}

void LZ4Decoder::init(const size_t blockSize)
{
    mBlockSize = blockSize;
    mTotalWritten = 0;
    mTotalRead = 0;
}

int LZ4Decoder::open(FILE* out)
{
    mOut.open(out);
    LZ4_setStreamDecode(&mStreamDecode, NULL, 0);
    return 0;
}

int LZ4Decoder::open(char** out, size_t* outlen)
{
    mOut.open(out, outlen);
    LZ4_setStreamDecode(&mStreamDecode, NULL, 0);
    return 0;
}

size_t LZ4Decoder::decode(FILE* in)
{
    mIn.open(in);
    return decode(-1);
}

size_t LZ4Decoder::decode(char* in, size_t inlen)
{
    mIn.open(in, inlen);
    return decode(inlen);
}

size_t LZ4Decoder::decode(size_t inlen)
{
	char *outBuf;
	outBuf = (char *)malloc(sizeof(size_t) * mBlockSize);
    //char outBuf[size];

	char *cmpBuf;
	cmpBuf = (char *)malloc(LZ4_COMPRESSBOUND(mBlockSize));
    //char cmpBuf[LZ4_COMPRESSBOUND(mBlockSize)];

	int inBlockSize;
    size_t remain = inlen;
    while (1)
    {
        size_t readCount = mIn.read(&inBlockSize, sizeof(inBlockSize));
        mTotalRead += readCount;
        if (inBlockSize <= 0 || readCount != sizeof(inBlockSize))
        {
            return 0;
        }
        size_t readCountBuf = mIn.read(cmpBuf, inBlockSize);
        mTotalRead += readCountBuf;
        if ((int)readCountBuf != inBlockSize)
        {
            break;
        }
        int outBlockSize = LZ4_decompress_safe_continue(&mStreamDecode, cmpBuf, outBuf, inBlockSize, mBlockSize);
        mTotalWritten += mOut.write(outBuf, outBlockSize);
        remain = inlen - outBlockSize;
        if (remain < mBlockSize)
        {
            break;
        }
    }
    return inlen - remain;
}

size_t LZ4Decoder::getTotalByteWritten() const
{
    return mTotalWritten;
}

size_t LZ4Decoder::getTotalByteRead() const
{
    return mTotalRead;
}

LZ4Decoder::~LZ4Decoder()
{
    // do nothing
}

