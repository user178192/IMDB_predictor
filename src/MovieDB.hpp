#ifndef _MOVIE_DB_
#define _MOVIE_DB_

#include <DataSchema.hpp>
#include <Index.hpp>
#include <string>
#include <limits>

namespace imdb {

using namespace std;

//split string into vector of strings, by any delimiter, with limitation,
//Lowercase every letter in the same time
vector<string> split_string(const string& s, const string& delim, size_t max_split=numeric_limits<size_t>::max());

class MovieDB
{
    //to do: split string function

    ReverseIndex ri_movie_;
    ReverseIndex ri_time_;
    ReverseIndex ri_people_;

public:

    Index<string, Movie> movies_;
    Index<string, People> actors_;
    Index<string, People> composers_;
    Index<string, People> directors_;


    int LoadFromFile(const std::string& filename);
    int SaveToFile(const std::string& filename);
};




}


#endif


