#ifndef _MOVIESIMILARITY_HPP_
#define _MOVIESIMILARITY_HPP_
#include <vector>
#include <string>
#include <Common.hpp>
#include <DataSchema.hpp>


namespace imdb {

    class MovieDB;

    static unsigned int simil_size = 10;
    static unsigned int composer_power = 9;

    size_t SimilarityScore(const Movie& ma, const Movie& mb);

    std::vector<size_t> GetSimilarMovies(const Movie& m, MovieDB *mdb);

}

#endif

int main(int argc, char *argv[]) {
	if (argc < 2) {
        return 1;
    }

    /*MovieDB *db = new MovieDB();
    chdir(argv[1]);
    db->LoadFromFile(argv[1]);
    db->BuildIndex();

    Similarity test;
    test.BuildSimilarity(db);
    return 0;*/
}