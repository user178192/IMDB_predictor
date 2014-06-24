#include <Parser.hpp>
#include <MovieDB.hpp>

#include <unistd.h>

using namespace imdb;

int main(int argc, char *argv[]) {
	if (argc < 2) {
        return 1;
    }

    MovieDB *db = new MovieDB();
    chdir(argv[1]);
    TypeTable testType;
    testType.init(db);
    testType.exec("movies.list");
    //testType.exec("genres.list");
    delete db;
    return 0;
}

