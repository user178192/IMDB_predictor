#include <Index.hpp>

#include <string>
#include <vector>
#include <iostream>

using namespace std;
using namespace imdb;

int main(int argc, char *argv[])
{
    struct Movie {
        vector<size_t> actors;
        vector<size_t> directors;
        string plot;
    };
    
    
    Index<string, Movie> movie_index;

    Movie ma;
    ma.plot = "Very simple plot 1\n";

    movie_index.Insert("movie_title_1", ma);
    ma.plot = "Very simple plot 2\n";
    movie_index.Insert("movie_title_2", ma);

    cout << "id for movie 1: " << get<1>(movie_index.GetID("movie_title_1")) << endl;

    cout << "info for movie 1: " << get<2>(movie_index.GetInfo("movie_title_1"))->plot << endl;

    cout << "info for movie 2: " << get<2>(movie_index.GetInfo(1))->plot << endl;

    cout << "exist for movie inexist title: " << get<0>(movie_index.GetInfo("sbsbsb")) << endl;

    cout << "exist for movie inexist id: " << get<0>(movie_index.GetInfo(3)) << endl;


    RevertIndex ri;

    ri.Insert({"edge", "of", "tomorrow"}, 1);
    ri.Insert({"edge", "and", "tomorrow"}, 2);
    ri.Insert({"edge", "and", "yesterday"}, 3);

    auto ret = ri.Lookup({"edge", "of", "tomorrow"});
    cout << "query 'edge of tomorrow': ";
    for(const auto i : ret) 
        cout << i << '\t';
    cout << endl;

    ret = ri.Lookup({"edge", "of", "yesterday"});
    cout << "query 'edge of yesterday': ";
    for(const auto i : ret) 
        cout << i << '\t';
    cout << endl;

    ret = ri.Lookup({"edge"});
    cout << "query 'edge': ";
    for(const auto i : ret) 
        cout << i << '\t';
    cout << endl;

    ret = ri.Lookup({"and"});
    cout << "query 'and': ";
    for(const auto i : ret) 
        cout << i << '\t';
    cout << endl;



    return 0;
}

