#ifndef _ZLIBCOMPRESSION_HPP_
#define _ZLIBCOMPRESSION_HPP_
#include <string>

std::string zlib_compress(const std::string& str,
                            int compressionlevel = 5);

std::string zlib_decompress(const std::string& str);

#endif

