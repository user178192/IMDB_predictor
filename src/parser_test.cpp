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
<<<<<<< HEAD
    //testType.exec("movies.list");
    testType.exec("actors.list");
=======
    testType.exec("movies.list");
    //auto d = new ActorsParser("sdfa", db);
    //d->parseLine("----\t");
    //d->parseLine("\t\t\t\"Catherine\" (1999) {To <<bi>> or not to <<bi>> (#3.5)}  [Thomas]");

    //db->SaveToFile(argv[1]);
>>>>>>> FETCH_HEAD
    delete db;
    return 0;
}

