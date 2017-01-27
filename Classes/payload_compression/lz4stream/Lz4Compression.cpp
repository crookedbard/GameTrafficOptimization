//
//  Lz4Compression.cpp
//  GameTrafficOptimization
//
//  Created by Dainius Kreivys on 2017-01-15.
//
//

#include "Lz4Compression.hpp"
//#if (CC_TARGET_PLATFORM != CC_PLATFORM_IOS)
#include "lz4_stream.h"
//#endif
#include <sstream>
#include "xxhash/xxhash.h"
#include <numeric>

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
#include "utils/ScreenLog.hpp"
#include "utils/Timer.h"
//#include "transport_protocols/tcp/Common.h"
#else
#include "ScreenLog.hpp"
#include "Timer.h"
#endif

std::string Lz4Compression::encode(std::string buffer, int& csize)
{
	std::stringstream compressed_stream;
	LZ4OutputStream lz4_out_stream(compressed_stream);
	std::stringstream input_stream(buffer);
	std::copy(std::istreambuf_iterator<char>(input_stream),
	          std::istreambuf_iterator<char>(),
	          std::ostreambuf_iterator<char>(lz4_out_stream));
	lz4_out_stream.close();

	compressed_stream.seekg(0, std::ios::end);
	csize = compressed_stream.tellg();

	return compressed_stream.str();
}

std::string Lz4Compression::decode(std::string buffer)
{
	std::stringstream compressed_stream(buffer);
	LZ4InputStream lz4_in_stream(compressed_stream);
	std::stringstream decompressed_stream;
	std::copy(std::istreambuf_iterator<char>(lz4_in_stream),
	          std::istreambuf_iterator<char>(),
	          std::ostreambuf_iterator<char>(decompressed_stream));
	return decompressed_stream.str();
}

/**
 * \brief Performs compression and decompression tests
 * \param cspeed - compression speed in MB/s will be returned on success
 * \param dspeed - compression speed in MB/s will be returned on success
 * \return - if success - return true
 */
bool Lz4Compression::performTests(int& cspeed, int& dspeed)
{
	try
	{
		cspeed = performCompressionTest();
		dspeed = performDecompressionTest();
	}
	catch (const std::exception& e)
	{
		printf("%s", e.what());
		return false;
	}

	return true;
}

size_t random_number(size_t max_size)
{
	std::random_device rd;
	std::mt19937 mt(rd());
	return std::uniform_int_distribution<size_t>(0, max_size)(mt);
}

std::string random_string2(size_t size)
{
	std::random_device rd;
	std::string str;
	str.reserve(size);
	std::generate_n(std::back_inserter(str), size, std::ref(rd));

	return str;
}

std::string random_string(size_t length)
{
	auto randchar = []() -> char
		{
			const char charset[] =
				"0123456789"
				"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
				"abcdefghijklmnopqrstuvwxyz";
			const size_t max_index = (sizeof(charset) - 1);
			return charset[rand() % max_index];
		};
	std::string str(length, 0);
	std::generate_n(str.begin(), length, randchar);
	return str;
}

unsigned long getTimeMillis2()
{
	timeval time;
	cocos2d::gettimeofday(&time, nullptr);
	unsigned long millisecs = (time.tv_sec * 1000) + (time.tv_usec / 1000);
	return millisecs;// static_cast<float>(millisecs);
}

/**
 * \brief performs compression test
 * \return  - speed MB/s
 */
int Lz4Compression::performCompressionTest()
{
	//meassure();
	auto size = 0;
	//auto now2 = Timer::GetTime();
	auto total_compressed_size = 0;
	const int CHUNK_SIZE = 1000 * 1000;
	const unsigned long LOOP_TIME_MS = 1000;
	std::stringstream compressed_stream;
	LZ4OutputStream lz4_out_stream(compressed_stream);
	auto now = getTimeMillis2();
	unsigned long d = 0.0f;
	while (true)
	{
		//int csize;
		//encode(random_string(1000 * 1000), csize);
		//auto genTimeStart = getTimeMillis2();
		auto buffer = random_string(CHUNK_SIZE);
		//std::stringstream input_stream(random_string(100));
		//auto genTimeEnd = getTimeMillis2() - genTimeStart;
		//now += genTimeEnd;
		int csize;
		auto timestart = getTimeMillis2();
		encode(buffer, csize);
		auto timeend = getTimeMillis2();
		d += (timeend - timestart);

		auto dif = d - now;
		if (dif > LOOP_TIME_MS) break;
//		d/*getTimeMillis2()*/ - now < (double)1000
//		std::copy(std::istreambuf_iterator<char>(input_stream),
//		          std::istreambuf_iterator<char>(),
//		          std::ostreambuf_iterator<char>(lz4_out_stream));
//		
//
//		compressed_stream.seekg(0, std::ios::end);
//		int csize = compressed_stream.tellg();

		size += CHUNK_SIZE;
		total_compressed_size += csize;
	}
	lz4_out_stream.close();

	printf("%d B/s", size);
	printf("%d B/s", total_compressed_size);
	return size;
}

/**
 * \brief performs decompression test
 * \return - speed MB/s
 */
int Lz4Compression::performDecompressionTest()
{
	//	auto now = Timer::GetTime();
	//	auto size = 0;
	//	auto total_decompressed_size = 0;
	//	while (Timer::GetTime() - now <= 1)
	//	{
	//		int csize;
	//		decode(random_string(1000), csize);
	//		size += 1000;
	//		total_compressed_size += csize;
	//	}
	//	printf("%.3f MB/s", size / 1000000);
	//	printf("%.3f MB/s", total_compressed_size / 1000000);
	//	return size;
	return 0;
}

//https://github.com/t-mat/lz4mt/blob/master/src/lz4mt_benchmark.cpp
namespace
{
	typedef std::chrono::high_resolution_clock Clock;
	typedef Clock::time_point TimePoint;

	TimePoint getTime()
	{
		return Clock::now();
	}

	TimePoint getSyncTime()
	{
		const auto t0 = getTime();
		for (;;)
		{
			const auto t = getTime();
			if (t0 != t)
			{
				return t;
			}
		}
	}

	double getTimeSpan(const TimePoint& tStart, const TimePoint& tEnd)
	{
		using namespace std::chrono;
		const auto dt = duration_cast<seconds>(tEnd - tStart).count();
		return static_cast<double>(dt);
	}
} // anonymous namespace
#include <vector>
#include <functional>
#include <fstream>

void Lz4Compression::meassure()
{
	std::string filename = "lz4test.txt";
	int nIter;
	std::vector<std::string> files;
	//std::function<bool(Lz4MtContext* ctx, const std::string& filename)> openIstream;
	//std::function<void(Lz4MtContext* ctx)> closeIstream;
	std::function<uint64_t(const std::string& filename)> getFilesize;

	size_t totalFileSize = 0;
	size_t totalCompressSize = 0;
	auto totalCompressTime = 0.0;
	auto totalDecompressTime = 0.0;
	const auto TIMELOOP = 2.0;

	

//	std::ifstream is;
//	is.open(filename);
//
//	printf("filename: %s", filename.c_str());
//
//	// get length of file:
//	is.seekg(0, is.end);
//	int length = is.tellg();
//	is.seekg(0, is.beg);
//	std::cout << "Reading " << length << " characters... ";

	//char * buffer = new char[length];
	std::stringstream buffer(random_string(10 * 1024 * 1024));

	std::vector<char> inpBuf(
		static_cast<size_t>(10 * 1024 * 1024)
	);
	
	// read data as a block:
	//is.read(buffer, length);

//	if (is)
//	{
//		buffer << is.rdbuf();
//		is.close();
//	}

	//char data[100];
	//		const size_t readSize = is->read(ctx, inpBuf.data()
	//			, static_cast<int>(inpBuf.size()));
	//closeIstream(ctx);
	// close the opened file.
	//is.close();

	//		if (inpBuf.size() != readSize) {
	//			//msgErrReading(filename);
	//			printf("error reading file: %s", filename);
	//			return;
	//		}

	const auto blockMaximumSize = 10 * 1024 * 1024;
	const auto inpHash =
		XXH32(inpBuf.data(), static_cast<int>(inpBuf.size()), 0);
	const auto chunkSize =
		(size_t(1) << (8 + (2 * blockMaximumSize)));
	const auto nChunk = (inpBuf.size() / chunkSize) + 1;
	const auto maxChunkSize = chunkSize; //?
	//			static_cast<size_t>(
	//				ctx->compressBound(static_cast<int>(chunkSize)));

	std::vector<char> outBuf(nChunk * maxChunkSize);

	struct Chunk
	{
		int id;
		char* inpPtr;
		size_t inpSize;
		char* outPtr;
		size_t outSize;
		size_t cmpSize;
		size_t decSize;
	};

	std::vector<Chunk> chunks;
//	(nChunk);
//	{
//		auto r = inpBuf.size();
//		for (auto& e : chunks)
//		{
//			const auto i = static_cast<int>(&e - chunks.data());
//			e.id = i;
//			e.inpPtr = &inpBuf[i * chunkSize];
//			e.inpSize = r > chunkSize ? chunkSize : r;
//			e.outPtr = &outBuf[i * maxChunkSize];
//			e.outSize = maxChunkSize;
//			e.cmpSize = 0;
//			e.decSize = 0;
//			r -= e.inpSize;
//		}
//	}

//	std::vector<std::future<void>> futures(chunks.size());

//	const auto b = [=, &futures, &chunks]
//		(std::function<void(Chunk*)> fChunk) -> double
//		{
//			const auto t0 = getSyncTime();
//			auto t1 = t0;
//			int loopCount = 0;
//
//			while (getTimeSpan(t0, t1 = getTime()) < TIMELOOP)
//			{
//				for (auto& e : chunks)
//				{
//					futures[e.id] = async(std::launch::async, fChunk, &e);
//				}
//				for (auto& e : futures)
//				{
//					e.wait();
//				}
//				++loopCount;
//			}
//
//			return getTimeSpan(t0, t1) / static_cast<double>(loopCount);
//		};

	auto minCmpTime = DBL_MAX;
	auto minDecTime = DBL_MAX;
	size_t cmpSize = 0;
	for (int iLoop = 0, nLoop = nChunk; iLoop <= nLoop; ++iLoop)
	{
		//			msgReport(filename, iLoop, inpBuf.size()
		//				, cmpSize, minCmpTime, minDecTime);
		printf("File = %s, loop = %d, %d, %d, %.f, %.f", filename.c_str(), iLoop, totalFileSize, totalCompressSize
		       , totalCompressTime, totalDecompressTime);

		// compression
		iota(outBuf.begin(), outBuf.end(), 0);
		//			const auto cmpTime = b(
		//				[ctx, singleThread, &futures](Chunk* cp) {
		//				if (singleThread && cp->id > 0) {
		//					futures[cp->id - 1].wait();
		//				}
		//				cp->cmpSize = ctx->compress(
		//					cp->inpPtr
		//					, cp->outPtr
		//					, static_cast<int>(cp->inpSize)
		//					, static_cast<int>(cp->outSize)
		//					, ctx->compressionLevel
		//				);
		//			}
		//			);
		Chunk fchunk;
		fchunk.id = iLoop;
		auto charBuff = new char[chunkSize];
		
		buffer.read(charBuff, chunkSize * iLoop);
		fchunk.inpPtr = charBuff;
		fchunk.inpSize = chunkSize;

		auto now = getTimeMillis2();
		//			std::stringstream compressed_stream;
		//			LZ4OutputStream lz4_out_stream(compressed_stream);
		//			std::stringstream input_stream(charBuff); 
		//			std::copy(std::istreambuf_iterator<char>(input_stream),
		//				std::istreambuf_iterator<char>(),
		//				std::ostreambuf_iterator<char>(lz4_out_stream));
		//			lz4_out_stream.close();
		int csize;
		auto cres = encode(charBuff, csize);
		auto cmpTime = getTimeMillis2() - now;

		fchunk.outPtr = const_cast<char *>(cres.c_str());
		fchunk.outSize = csize;
		fchunk.cmpSize = fchunk.inpSize - fchunk.outSize;

		chunks.push_back(fchunk);

		minCmpTime = std::min(minCmpTime, double(cmpTime));

		if (0 == iLoop)
		{
			cmpSize = 0;
			for (const auto& c : chunks)
			{
				cmpSize += c.cmpSize;
			}
		}

		//			msgReport(filename, iLoop, inpBuf.size()
		//				, cmpSize, minCmpTime, minDecTime);
		printf("File = %s, loop = %d, %d, %d, %.f, %.f", filename.c_str(), iLoop, totalFileSize, totalCompressSize
		       , totalCompressTime, totalDecompressTime);

		// decompression
		fill(inpBuf.begin(), inpBuf.end(), 0);
		//			const auto decTime = b(
		//				[ctx, singleThread, &futures](Chunk* cp) {
		//				if (singleThread && cp->id > 0) {
		//					futures[cp->id - 1].wait();
		//				}
		//				cp->decSize = ctx->decompress(
		//					cp->outPtr
		//					, cp->inpPtr
		//					, static_cast<int>(cp->cmpSize)
		//					, static_cast<int>(cp->inpSize)
		//				);
		//			}
		//			);

		auto now2 = getTimeMillis2();
		auto dstr2 = Lz4Compression::decode(fchunk.outPtr);
		double decTime = getTimeMillis2() - now2;
		minDecTime = std::min(minDecTime, decTime);

		fchunk.decSize = dstr2.length();

		//			msgReport(filename, iLoop, inpBuf.size()
		//				, cmpSize, minCmpTime, minDecTime);
		printf("File = %s, loop = %d, %d, %d, %.f, %.f", filename.c_str(), iLoop, totalFileSize, totalCompressSize
		       , totalCompressTime, totalDecompressTime);

		const auto outHash =
			XXH32(inpBuf.data(), static_cast<int>(inpBuf.size()), 0);

		if (inpHash != outHash)
		{
			//msgErrChecksum(filename, inpHash, outHash);
			printf("msgErrChecksum");
			break;
		}
	}


	totalFileSize += inpBuf.size();
	totalCompressSize += cmpSize;
	totalCompressTime += minCmpTime;
	totalDecompressTime += minDecTime;

	printf("TOTAL %d, %d, %.f, %.f", totalFileSize, totalCompressSize
	       , totalCompressTime, totalDecompressTime);
	//		msgReport("  TOTAL", 0, totalFileSize, totalCompressSize
	//			, totalCompressTime, totalDecompressTime);
}
