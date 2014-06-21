#include "Parser.hpp"
#include "Log.hpp"
#include <MovieDB.hpp>
#include <stdio.h>
#include <climits>
#include <cstring>

using namespace imdb;

void ComposersParser::Init() {
}

std::string ComposersParser::splitComposersName(const std::string& input_line) {
    size_t pos1 = 0, pos2 = 0;
    while (input_line[pos2] != '\t') {
        pos2++;
    }
    
    std::string composer_name(input_line, pos1, pos2);
    splitMoiveName(pos2, composer_name, input_line);
    return composer_name;
}

void ComposersParser::Finish() {
    LOG_INFO("Read in %llu Composers", db_->composers_.Size());
}

void ComposersParser::insertDB(const std::string& composer_name, const std::string& movie_name) {
    auto com_key = composer_name;
    auto mov_key = movie_name;
    size_t com_id = 0;

    auto com_obj = db_ -> composers_.GetInfo(com_key); // here is the composers info
    auto mov_obj = db_ -> movies_.GetInfo(mov_key);

    if (!get<0>(mov_obj)) {
        LOG_DEBUG("Movie [%s] not found, inconsistant", movie_name.c_str());
        return;
    }

    if (get<0>(com_obj)) {
        // the composer already exists, insert the movie id to the Composer
        get<2>(com_obj)->movies_.push_back(get<1>(mov_obj));
    } else {
        // insert new composer
        Composer c;
        c.movies_.push_back(get<1>(mov_obj));
        com_id = db_->composers_.Insert(com_key, c);
    }
    get<2>(mov_obj)->composer_ = com_id;
}

/*
    For Television Series 
    Not consider the subtitle
*/

void ComposersParser::splitMoiveName(const size_t begin, const std::string& composer_name, const std::string& input_line) {
    size_t left_pos = begin;

    while (input_line[left_pos] == '\t') {
        left_pos++;
    }

    std::string movie_name;
    size_t start = left_pos;
    size_t end = find_year_pos(input_line, start);        
    movie_name.assign(input_line, start, end - start + 1);
    if (movie_name[0] == '\"') {
        // For tv title , remove the "
        movie_name.erase (std::remove(movie_name.begin(), movie_name.end(), '\"'), movie_name.end());
    }
    insertDB(composer_name, movie_name);
}

void ComposersParser::parseLine(const std::string input_line) {
    Init();
    if (strncmp(input_line.c_str(), "----\t", 5) == 0) {
        begin_parse_ = true;
        return;
    } else if (strncmp(input_line.c_str(), "-----", 5) == 0) {
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
            splitMoiveName(0, composer_name_, input_line);
        }            // This line is Composers name + movie name
        else {
            composer_name_ = std::move(splitComposersName(input_line));
        }
    }
}

