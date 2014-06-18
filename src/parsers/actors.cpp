#include "Parser.hpp"
#include <MovieDB.hpp>

#include <cstring>

using namespace imdb;

void ActorsParser::Init() {
    begin_parse_ = false;
    actor_name_ = series_name1_ 
                = series_name2_
                = series_time_
                = actor_rank_
                = movie_name_
                = movie_time_
                = "";
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
        size_t right_pos = input_line.find('"', left_pos + 1);
        series_name1_.assign(input_line, left_pos + 1, right_pos - left_pos - 1);

        left_pos = input_line.find('(', right_pos + 1);
        if (left_pos != std::string::npos) {
            right_pos = input_line.find(')', left_pos + 1);
            series_time_.assign(input_line, left_pos + 1, right_pos - left_pos - 1);
        }

        left_pos = input_line.find('{', right_pos + 1);
        if (left_pos != std::string::npos) {
            right_pos = input_line.find('}', left_pos + 1);
            series_name2_.assign(input_line, left_pos + 1, right_pos - left_pos - 1);
        }

        left_pos = input_line.find('<', right_pos + 1);
        if (left_pos != std::string::npos) {
            right_pos = input_line.find('>', left_pos + 1);
            actor_rank_.assign(input_line, left_pos + 1, right_pos - left_pos - 1);
        }

        //debug(actor_name + series_name1);
    }
        // it is a Movie
    else {

        size_t right_pos = input_line.find('(', left_pos + 1);

        // Take the case like Title (????)
        if (isdigit(input_line[right_pos + 1]) || input_line[right_pos + 1] == '?') {
            movie_name_.assign(input_line, left_pos, right_pos - left_pos - 1);
        } else { // Take the case like title (title) (year)
            right_pos = input_line.find(')', right_pos + 1);
            movie_name_.assign(input_line, left_pos, right_pos - left_pos - 1);

        }

        left_pos = input_line.find('(', right_pos);
        right_pos = input_line.find(')', left_pos + 1);
        movie_time_.assign(input_line, left_pos + 1, right_pos - left_pos - 1);

        left_pos = input_line.find('<', right_pos + 1);
        if (left_pos != std::string::npos) {
            right_pos = input_line.find('>', left_pos + 1);
            actor_rank_.assign(input_line, left_pos + 1, right_pos - left_pos - 1);
        }
    }
}

void ActorsParser::parseLine(const std::string input_line) {
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

