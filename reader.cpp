#include "reader.hpp"

// load the database to the reader

template <typename T>
auto debug(T t) -> decltype(t) {
    std::cout << t << std::endl;
    return t;
}

void Reader::parseFile(const std::string file_name) {
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

std::string ActorsReader::splitActorsName(const std::string& input_line) {
    size_t pos1 = 0, pos2 = 0;
    while (input_line[pos2] != '\t') {
        pos2++;
    }
    std::string actor_name(input_line, pos1, pos2);
    debug(actor_name);
    splitMoiveName(pos2, input_line);
    return actor_name;
}

std::vector<std::string> ActorsReader::splitMoiveName(const size_t begin, const std::string& input_line) {
    size_t left_pos = begin;
    std::vector<std::string> res;

    while (input_line[left_pos] == '\t') {
        left_pos++;
    }

    // it is a Television Series 
    if (input_line[left_pos] == '"') {
        size_t right_pos = input_line.find('"', left_pos + 1);
        std::string series_name1(input_line, left_pos + 1, right_pos - left_pos - 1);

        left_pos = input_line.find('(', right_pos + 1);
        if (left_pos != std::string::npos) {
            right_pos = input_line.find(')', left_pos + 1);
            std::string series_time(input_line, left_pos + 1, right_pos - left_pos - 1);
        }

        left_pos = input_line.find('{', right_pos + 1);
        if (left_pos != std::string::npos) {
            right_pos = input_line.find('}', left_pos + 1);
            std::string series_name2(input_line, left_pos + 1, right_pos - left_pos - 1);
        }

        left_pos = input_line.find('<', right_pos + 1);
        if (left_pos != std::string::npos) {
            right_pos = input_line.find('>', left_pos + 1);
            std::string actor_rank(input_line, left_pos + 1, right_pos - left_pos - 1);
        }
        return res;
    }
        // it is a Movie
    else {

        size_t right_pos = input_line.find('(', left_pos + 1);

        // Take the case like Title (????)
        if (isdigit(input_line[right_pos + 1]) || input_line[right_pos + 1] == '?') {
            std::string movie_name(input_line, left_pos, right_pos - left_pos - 1);
        } else { // Take the case like title (title) (year)
            right_pos = input_line.find(')', right_pos + 1);
            std::string movie_name(input_line, left_pos, right_pos - left_pos - 1);
        }

        left_pos = input_line.find('(', right_pos);
        right_pos = input_line.find(')', left_pos + 1);
        std::string movie_time(input_line, left_pos + 1, right_pos - left_pos - 1);
        return res;
    }
}

void ActorsReader::parseLine(const std::string input_line) {
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
        }            // This line is movie name
        else if (*(input_line.begin()) == '\t') {
            splitMoiveName(0, input_line);
        }
            // This line is actors name + movie name
        else {
            splitActorsName(input_line);
        }
    }
}

// should delete the object in the end

void TypeTable::insert(const std::string file_name) {

    if (file_name == "actors.list") {
        parser_map[file_name] = new ActorsReader(file_name);
    }
    else if (file_name == "actresses.list") {
        parser_map[file_name] = new ActressesReader(file_name);
    }
    else if (file_name == "keywords.list") {
        parser_map[file_name] = new KeywordsReader(file_name);
    }
    else if (file_name == "directors.list") {
        parser_map[file_name] = new DirectorsReader(file_name);
    }
}

void TypeTable::exec(const std::string file_name) {
    assert(parser_map.count(file_name));
    parser_map[file_name] -> parseFile(file_name);
}

int main(int argc, char *argv[]) {
    TypeTable testType;
    testType.insert("actors.list");
    testType.exec("actors.list");
    return 0;
}