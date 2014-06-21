#include <Parser.hpp>
#include <MovieDB.hpp>
#include <Log.hpp>

// load the database to the Parser
using namespace imdb;

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

size_t Parser::find_year_pos(const std::string input_line, size_t begin) {
        size_t flag = begin;
        size_t left_pos = begin, right_pos = begin;
        for(size_t index = begin ; index < input_line.length(); index++) {
            if (input_line[index] == '(') {left_pos = index;}
            if (input_line[index] == ')') {right_pos = index;}

            if (right_pos > left_pos && right_pos - left_pos >= 5) {
                std::string temp(input_line, left_pos + 1, right_pos - left_pos - 1);
                // (1990/IV)
                if (temp.length() > 4) {
                    if (temp.find_first_not_of("0123456789?/IVX") == std::string::npos) {
                        flag = right_pos;
                        break;
                    }
                }
                else if (temp.length() == 4) {
                    if (temp.find_first_not_of("0123456789?") == std::string::npos) {
                        flag = right_pos;
                        break;
                    }
                }
            }
        }
        return flag;       
}

std::vector<std::string> Parser::split(const std::string &input_line, const char &delim ) {
    auto begin = input_line.begin();
    vector<string> tokens;

    while(begin < input_line.end()) {
        auto temp = find(begin, input_line.end(), delim);
        if(begin < input_line.end()) {
            tokens.push_back(string(begin, temp));
        }

        begin = temp;
        while ((begin < input_line.end()) && (*begin == delim)) {
            begin++;
        }
    }
    return tokens;
}

void TypeTable::init(MovieDB *db) {
    // the actors and actresses are same
    parser_map["actors.list"] = new ActorsParser("actors.list", db);
    parser_map["actresses.list"] = new ActorsParser("actresses.list", db);

    parser_map["genres.list"] = new GenresParser("genres.list", db);
    parser_map["directors.list"] = new DirectorsParser("directors.list", db);
    parser_map["composers.list"] = new ComposersParser("composers.list", db);
    parser_map["movies.list"] = new MoviesParser("movies.list", db);
    parser_map["language.list"] = new LanguagesParser("language.list", db);
    parser_map["countries.list"] = new CountriesParser("countries.list", db);
    parser_map["running-times.list"] = new RunningtimeParser("running-times.list", db);
    parser_map["ratings.list"] = new RatingParser("ratings.list", db);
}

void TypeTable::exec(const std::string file_name) {
    assert(parser_map.count(file_name));
    parser_map[file_name] -> parseFile(file_name);
}



