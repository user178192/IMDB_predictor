#ifndef _MOVIESIMILARITY_HPP_
#define _MOVIESIMILARITY_HPP_
#include <vector>
#include <string>
#include <Common.hpp>
#include <DataSchema.hpp>


namespace imdb {

    class MovieDB;

    size_t SimilarityScore(const Movie& ma, const Movie& mb);

    std::vector<size_t> GetSimilarMovies(const Movie& m, MovieDB *mdb);

}

#endif
