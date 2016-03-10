#include <parsers/Parser.hpp>
#include <common/MovieDB.hpp>

#include <unistd.h>

using namespace imdb;

int main(int argc, char *argv[]) {
	if (argc < 3) {
        return 1;
    }

    MovieDB *db = new MovieDB();
    chdir(argv[1]);
    TypeTable testType;
    testType.init(db);
    testType.exec(argv[2]);
    delete db;
    return 0;
}

