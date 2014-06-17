#include "Parser.hpp"

// load the database to the Parser

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

std::string ActorsParser::splitActorsName(const std::string& input_line) {
    size_t pos1 = 0, pos2 = 0;
    while (input_line[pos2] != '\t') {
        pos2++;
    }
    std::string actor_name(input_line, pos1, pos2);
    splitMoiveName(pos2, actor_name, input_line);
    return actor_name;
}

std::vector<std::string> ActorsParser::splitMoiveName(const size_t begin, const std::string& actor_name, const std::string& input_line) {
    size_t left_pos = begin;
    std::vector<std::string> res;
    std::string series_name1, series_name2, series_time, actor_rank;

    while (input_line[left_pos] == '\t') {
        left_pos++;
    }

    // it is a Television Series 
    if (input_line[left_pos] == '"') {
        size_t right_pos = input_line.find('"', left_pos + 1);
        series_name1.assign(input_line, left_pos + 1, right_pos - left_pos - 1);

        left_pos = input_line.find('(', right_pos + 1);
        if (left_pos != std::string::npos) {
            right_pos = input_line.find(')', left_pos + 1);
            series_time.assign(input_line, left_pos + 1, right_pos - left_pos - 1);
        }

        left_pos = input_line.find('{', right_pos + 1);
        if (left_pos != std::string::npos) {
            right_pos = input_line.find('}', left_pos + 1);
            series_name2.assign(input_line, left_pos + 1, right_pos - left_pos - 1);
        }

        left_pos = input_line.find('<', right_pos + 1);
        if (left_pos != std::string::npos) {
            right_pos = input_line.find('>', left_pos + 1);
            actor_rank.assign(input_line, left_pos + 1, right_pos - left_pos - 1);
        }

        //debug(actor_name + series_name1);
        return res;
    }
        // it is a Movie
    else {

        std::string movie_name, actor_rank;
        size_t right_pos = input_line.find('(', left_pos + 1);

        // Take the case like Title (????)
        if (isdigit(input_line[right_pos + 1]) || input_line[right_pos + 1] == '?') {
            movie_name.assign(input_line, left_pos, right_pos - left_pos - 1);
        } else { // Take the case like title (title) (year)
            right_pos = input_line.find(')', right_pos + 1);
            movie_name.assign(input_line, left_pos, right_pos - left_pos - 1);

        }

        left_pos = input_line.find('(', right_pos);
        right_pos = input_line.find(')', left_pos + 1);
        std::string movie_time(input_line, left_pos + 1, right_pos - left_pos - 1);

        left_pos = input_line.find('<', right_pos + 1);
        if (left_pos != std::string::npos) {
            right_pos = input_line.find('>', left_pos + 1);
            actor_rank.assign(input_line, left_pos + 1, right_pos - left_pos - 1);
        }

        //debug(actor_name + " | " + movie_name + "(" + movie_time + ")" + " | " + actor_rank);

        return res;
    }
}

void ActorsParser::parseLine(const std::string input_line) {
    // begin parser in line 239, actors.list
    if (line_num_ == 239) {
        begin_parse_ = true;
    }
        // stop parser in line 15835158, actors.list
    else if (line_num_ == 15835158) {
        // stop parser in line 15835158, actors.list
        begin_parse_ = false;
    }

    if (begin_parse_ == true) {
        // This line is empty line
        if (input_line.length() == 0) {
            return;
        }  
        // This line is movie name
        else if (*(input_line.begin()) == '\t') {
            splitMoiveName(0, actor_name_, input_line);
        }
        // This line is actors name + movie name
        else {
            actor_name_ = std::move(splitActorsName(input_line));
        }
    }
}

// should delete the object in the end

void TypeTable::init() {
        parser_map["actors.list"] = new ActorsParser("actors.list");
        parser_map["actresses.list"] = new ActressesParser("actresses.list");
        parser_map["keywords.list"] = new KeywordsParser("keywords.list");
        parser_map["directors.list"] = new DirectorsParser("directors.list");
        parser_map["movies.list"] = new MoviesParser("movies.list");
}

void TypeTable::exec(const std::string file_name) {
    assert(parser_map.count(file_name));
    parser_map[file_name] -> parseFile(file_name);
}

int main(int argc, char *argv[]) {
    TypeTable testType;
    testType.init();
    testType.exec("actors.list");
    return 0;
}