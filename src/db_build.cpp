#include <parsers/Parser.hpp>
#include <common/MovieDB.hpp>
#include <common/Log.hpp>

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
    LOG_DEBUG("finish exec rating.list\n");
    testType.exec("movies.list");
    LOG_DEBUG("finish exec movie.list\n");
    testType.exec("actors.list");
    LOG_DEBUG("finish exec actor.list\n");
    testType.exec("actresses.list");
    LOG_DEBUG("finish exec actress.list\n");
    testType.exec("composers.list");
    LOG_DEBUG("finish exec composer.list\n");
    testType.exec("directors.list");
    LOG_DEBUG("finish exec director.list\n");
    testType.exec("language.list");
    LOG_DEBUG("finish exec language.list\n");
    testType.exec("countries.list");
    LOG_DEBUG("finish exec country.list\n");
    testType.exec("genres.list");
    LOG_DEBUG("finish exec genres.list\n");
    testType.exec("running-times.list");
    LOG_DEBUG("finish exec running-times.list\n");

    db->SaveToFile(argv[1]);
    delete db;
    return 0;
}

