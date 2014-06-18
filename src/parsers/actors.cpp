#include "Parser.hpp"
#include <MovieDB.hpp>
#include <stdio.h>

using namespace imdb;

void ActorsParser::Init() {
    bool begin_parse_ = false;
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

void ActorsParser::splitMoiveName(const size_t begin, const std::string& actor_name, const std::string& input_line) {
    size_t left_pos = begin;

    while (input_line[left_pos] == '\t') {
        left_pos++;
    }

    // it is a Television Series 
    if (input_line[left_pos] == '"') {

        std::string series_name1, series_name2, series_time, actor_rank;
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
    }
        // it is a Movie
    else {

        size_t right_pos = input_line.find('(', left_pos + 1);
        std::string movie_name, movie_time, actor_rank;

        // Take the case like Title (????)
        if (isdigit(input_line[right_pos + 1]) || input_line[right_pos + 1] == '?') {
            movie_name.assign(input_line, left_pos, right_pos - left_pos - 1);
        } else { // Take the case like title (title) (year)
            right_pos = input_line.find(')', right_pos + 1);
            movie_name.assign(input_line, left_pos, right_pos - left_pos - 1);

        }

        left_pos = input_line.find('(', right_pos);
        right_pos = input_line.find(')', left_pos + 1);
        movie_time.assign(input_line, left_pos + 1, right_pos - left_pos - 1);

        left_pos = input_line.find('<', right_pos + 1);
        if (left_pos != std::string::npos) {
            right_pos = input_line.find('>', left_pos + 1);
            actor_rank.assign(input_line, left_pos + 1, right_pos - left_pos - 1);
        }
    }
}

void ActorsParser::parseLine(const std::string input_line) {
    Init();
    if ( strncmp(input_line.c_str(), "----\t", 5) == 0) {
        begin_parse_ = true;
		return;
    }
    else if ( strncmp(input_line.c_str(), "-----", 5) == 0) {
        begin_parse_ = false;
		return;
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

