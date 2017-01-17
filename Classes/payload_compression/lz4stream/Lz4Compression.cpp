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

std::string Lz4Compression::encode(std::string buffer)
{
	std::stringstream compressed_stream;
	LZ4OutputStream lz4_out_stream(compressed_stream);
	std::stringstream input_stream(buffer);
	std::copy(std::istreambuf_iterator<char>(input_stream),
		std::istreambuf_iterator<char>(),
		std::ostreambuf_iterator<char>(lz4_out_stream));
	lz4_out_stream.close();
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
