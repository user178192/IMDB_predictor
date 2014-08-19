#include <parsers/Parser.hpp>
#include <common/MovieDB.hpp>

#include <unistd.h>

using namespace imdb;

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s db.filename\n", argv[0]);
        return 1;
    }



    MovieDB *db = new MovieDB();
    db->LoadFromFile(argv[1]);
    db->SaveToFile(argv[1]);
    delete db;
    return 0;
}

