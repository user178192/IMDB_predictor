#ifndef _MOVIE_DB_
#define _MOVIE_DB_

#include <DataSchema.hpp>
#include <Index.hpp>
#include <string>

namespace imdb {

    using namespace std;

    class MovieDB {
        //to do: split string function
    public:

        Index<string, Movie> movies_;
        Index<string, People> actors_;
        Index<string, People> composers_;
        Index<string, People> directors_;

        ReverseIndex ri_movie_;
        ReverseIndex ri_time_;
        ReverseIndex ri_people_;

        void BuildIndex();
        int LoadFromFile(const std::string& filename);
        int SaveToFile(const std::string& filename);
    };




}


#endif


