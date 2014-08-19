#include <parsers/Parser.hpp>
#include <common/MovieDB.hpp>
#include <common/Log.hpp>
#include <relation/MovieRelation.hpp>

#include <unistd.h>

using namespace imdb;

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s db.filename\n", argv[0]);
        return 1;
    }

    MovieDB *db = new MovieDB();
    db->LoadFromFile(argv[1]);

    MovieRelation::GetAllSimilarity(db);
    MovieRelation::GetAllRatingPred(db);

    db->SaveToFile(argv[1]);
    delete db;
    return 0;
}

