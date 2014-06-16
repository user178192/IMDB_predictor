#ifndef _MOVIE_HPP_
#define _MOVIE_HPP_
#include <vector>
#include <string>

namespace imdb {


struct Movie
{
    using namespace std;

    /* ratings.list */
    double rate_;
    size_t votes_; 
    
    /* ???? */
    vector<size_t> actors_;

    /* genres.list */
    vector<size_t> genres_;

    /* directors.list */
    vector<size_t> directors_;

    /* composers.list */
    size_t composer;

    /* countries.list */
    vector<string> countries_;

    /* language.list */
    vector<string> languages_;

    /* plots.list */
    vector<string> plots_;

    /* running-times.list */
    int length_;
};



struct People
{
    using namespace std;
    vector<size_t> movies_;
};


typedef People Actor;
typedef People Composer;
typedef People Director;


};



#endif

