#ifndef _COMMON_HPP_
#define _COMMON_HPP_
#include <string>
#include <vector>
#include <limits>

namespace imdb {

using namespace std;

static const size_t NULLID = -1;

//split string into vector of strings, by any delimiter, with limitation,
//Lowercase every letter in the same time
vector<string> split_string(const string& s, const string& delim, size_t max_split = numeric_limits<size_t>::max());


}

#endif

