#include <Parser.hpp>
#include <MovieDB.hpp>

#include <unistd.h>

using namespace imdb;

int main(int argc, char *argv[]) {
    if (argc < 3) {
        fprintf(stderr, "Usage: %s db.filename rawfile.dir\n", argv[0]);
        return 1;
    }
    chdir(argv[2]);

    MovieDB *db = new MovieDB();
    TypeTable testType;
    testType.init(db);
    testType.exec("ratings.list");

    //db->SaveToFile(argv[1]);
    delete db;
    return 0;
}

