#include "Parser.hpp"
#include <MovieDB.hpp>

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
}


// should delete the object in the end
class TypeTable {
private:
    // map of data filename relate to Parser
    std::unordered_map<std::string, Parser*> parser_map;

public:
    TypeTable() = default;
    void init(MovieDB *db)
    {
        parser_map["actors.list"] = new ActorsParser("actors.list", db);
        parser_map["actresses.list"] = new ActressesParser("actresses.list", db);
        parser_map["keywords.list"] = new KeywordsParser("keywords.list", db);
        parser_map["directors.list"] = new DirectorsParser("directors.list", db);
        parser_map["movies.list"] = new MoviesParser("movies.list", db);
    }

    void exec(const std::string file_name)
    {
        assert(parser_map.count(file_name));
        parser_map[file_name] -> parseFile(file_name);
    }
};


#include <unistd.h>


int main(int argc, char *argv[]) {
    if (argc < 3) {
        fprintf(stderr, "Usage: %s db.filename rawfile.dir\n", argv[0]);
        return 1;
    }
    chdir(argv[2]);

    MovieDB *db = new MovieDB();
    TypeTable testType;
    testType.init(db);
    testType.exec("actors.list");

    db->SaveToFile(argv[1]);
    delete db;
    return 0;
}

