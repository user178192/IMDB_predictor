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

        ReverseIndex ri_movies_;
        ReverseIndex ri_actors_;
        ReverseIndex ri_composers_;
        ReverseIndex ri_directors_;

        void BuildIndex();
        int LoadFromFile(const string& filename);
        int SaveToFile(const string& filename);
        static string NameReorder(const string& name);
    };




}


#endif


