#include <Parser.hpp>
#include <MovieDB.hpp>
#include <Log.hpp>

// load the database to the Parser
using namespace imdb;

template <typename T>
auto debug(T t) -> decltype(t) {
    std::cout << t << std::endl;
    return t;
}

void Parser::parseFile(const std::string file_name) {
    std::string line;
    std::ifstream inputfile(file_name);
    if (inputfile.is_open()) {
        while (getline(inputfile, line)) {
            parseLine(line);
            line_num_++;
        }
        inputfile.close();
    }
    LOG_INFO("Parse %s done", file_name.c_str());
    Finish();
}

void TypeTable::init(MovieDB *db) {
    // the actors and actresses are same
    parser_map["actors.list"] = new ActorsParser("actors.list", db);
    parser_map["actresses.list"] = new ActorsParser("actresses.list", db);

    parser_map["genres.list"] = new GenresParser("genres.list", db);
    parser_map["directors.list"] = new DirectorsParser("directors.list", db);
    parser_map["composers.list"] = new DirectorsParser("composers.list", db);
    parser_map["movies.list"] = new MoviesParser("movies.list", db);
    parser_map["language.list"] = new LanguagesParser("language.list", db);
    parser_map["countries.list"] = new CountriesParser("countries.list", db);
    parser_map["running-times.list"] = new RunningtimeParser("running-times.list", db);
}

void TypeTable::exec(const std::string file_name) {
    assert(parser_map.count(file_name));
    parser_map[file_name] -> parseFile(file_name);
}



