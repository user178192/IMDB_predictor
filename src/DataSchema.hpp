#ifndef _DATASCHEMA_HPP_
#define _DATASCHEMA_HPP_
#include <vector>
#include <string>

namespace imdb {
    using namespace std;

    struct Movie {
        /* ratings.list */
        double rating_;
        size_t votes_;

        // movies.list,  like 1992-2013
        string year_;

        // movie type
        string type_;

        // for series
        vector<string> subtitles_;

        /* actors and actress */
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
        vector<string> length_;


        // For convenience
        void AddActor(size_t id, int pos);
        void AddDirector(size_t id);

        // sort the actors by rank and put them into actors list
        void RankPeople();

    private:
        vector<pair<size_t, int>> actors_rank_;
    };

    struct People {
        vector<size_t> movies_;
    };


    typedef People Actor;
    typedef People Composer;
    typedef People Director;


}



#endif

