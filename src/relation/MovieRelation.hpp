#ifndef _MOVIERELATION_HPP_
#define _MOVIERELATION_HPP_
#include <vector>
#include <string>
#include <common/Common.hpp>
#include <common/DataSchema.hpp>


namespace imdb {

    class MovieDB;

    class MovieRelation 
    {
    public:
        static const unsigned int simil_size = 10;
        static const unsigned int composer_power = 9;

        static size_t SimilarityScore(const Movie& ma, const Movie& mb);

        static std::vector<size_t> GetSimilarMovies(const Movie& m, MovieDB *mdb);

        static void GetAllSimilarity(MovieDB *mdb);
        static void GetAllRatingPred(MovieDB *mdb);
    };
}

#endif
