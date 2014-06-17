#ifndef _DATASCHEMA_HPP_
#define _DATASCHEMA_HPP_
#include <vector>
#include <string>

namespace imdb {
using namespace std;

struct Movie
{

    /* ratings.list */
    double rate_;
    size_t votes_; 
    
    /* ???? */
    vector<size_t> actors_;

    /* genres.list */
    vector<string> genres_;

    /* directors.list */
    vector<size_t> directors_;

    /* composers.list */
    size_t composer_;

    /* countries.list */
    vector<string> countries_;

    /* language.list */
    vector<string> languages_;

    /* plots.list */
    vector<string> plots_;

    /* running-times.list */
    int length_;



// For convenience
    void AddActor(size_t id, size_t pos);
    void AddDirector(size_t id, size_t pos);
};



struct People
{
    vector<size_t> movies_;
};


typedef People Actor;
typedef People Composer;
typedef People Director;


}



#endif

