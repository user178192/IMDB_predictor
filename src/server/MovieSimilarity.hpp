#ifndef _MOVIESIMILARITY_HPP_
#define _MOVIESIMILARITY_HPP_
#include <vector>
#include <string>
#include <common/Common.hpp>
#include <common/DataSchema.hpp>


namespace imdb {

    class MovieDB;

    static unsigned int simil_size = 10;
    static unsigned int composer_power = 9;

    size_t SimilarityScore(const Movie& ma, const Movie& mb);

    std::vector<size_t> GetSimilarMovies(const Movie& m, MovieDB *mdb);

}

#endif
