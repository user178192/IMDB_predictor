#ifndef _COMMON_HPP_
#define _COMMON_HPP_
#include <string>
#include <vector>
#include <limits>
#include <fstream>

namespace imdb {

using namespace std;

static const size_t NULLID = -1;

//split string into vector of strings, by any delimiter, with limitation,
//Lowercase every letter in the same time
vector<string> split_string(const string& s, const string& delim, size_t max_split = numeric_limits<size_t>::max());

// split string into vector of strings, by any delimiter, can split by mutil-delim input 
std::vector<std::string> split(const std::string &input_line, const char &delim );

// find fisrt vaild year input
size_t find_year_pos(const std::string input_line, size_t begin);
}


#endif

