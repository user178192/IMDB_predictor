#include "Parser.hpp"
#include "Log.hpp"
#include <MovieDB.hpp>
#include <stdio.h>

#include <cstring>

using namespace imdb;

void ComposersParser::Init() {
}

std::string ComposersParser::splitComposersName(const std::string& input_line) {
    size_t pos1 = 0, pos2 = 0;
    while (input_line[pos2] != '\t') {
        pos2++;
    }
    
    std::string Composer_name(input_line, pos1, pos2);
    splitMoiveName(pos2, Composer_name, input_line);
    return Composer_name;
}

void ComposersParser::Finish() {
    LOG_INFO("Read in %llu Composers", db_->Composers_.Size());
}

void ComposersParser::insertDB(const std::string& Composer_name, const std::string& movie_name) {
    auto dir_key = Composer_name;
    auto mov_key = movie_name;
    size_t dir_id = 0;

    auto act_obj = db_ -> Composers_.GetInfo(dir_key); // here is the Composers info
    auto mov_obj = db_ -> movies_.GetInfo(mov_key);

    if (!get<0>(mov_obj)) {
        LOG_DEBUG("Movie [%s] not found, inconsistant", movie_name.c_str());
        return;
    }

    if (get<0>(act_obj)) {
        // the Composer already exists, insert the movie id to the Composer
        get<2>(act_obj)->movies_.push_back(get<1>(mov_obj));
        get<2>(mov_obj)->AddComposer(dir_id);

    } else {
        // insert new Composer
        Composer c;
        c.movies_.push_back(get<1>(mov_obj));
        dir_id = db_->Composers_.Insert(dir_key, c);
        get<2>(mov_obj)->AddComposer(dir_id);
    }
}

/*
    For Television Series 
    Not consider the subtitle
*/

void ComposersParser::splitMoiveName(const size_t begin, const std::string& Composer_name, const std::string& input_line) {
    size_t left_pos = begin;

    while (input_line[left_pos] == '\t') {
        left_pos++;
    }

    // it is a Television Series 
    if (input_line[left_pos] == '"') {

        std::string series_name, series_time, Composer_rank;
        size_t right_pos = input_line.find('"', left_pos + 1);


        series_name.assign(input_line, left_pos + 1, right_pos - left_pos - 1);

        left_pos = input_line.find('(', right_pos + 1);
        if (left_pos != std::string::npos) {
            right_pos = input_line.find(')', left_pos + 1);
            series_time.assign(input_line, left_pos + 1, right_pos - left_pos - 1);
        }

        insertDB(Composer_name, series_name + " " + "(" + series_time + ")");
    }
    
    // it is a Movie
    else {
        size_t right_pos = 0;
        while (right_pos < input_line.length() ) { // find the end of year   
            if (input_line[right_pos] == ')') {
                //special case like (1999/II)
                size_t close = input_line.rfind('(', right_pos);
                if (isdigit(input_line[close + 1])) {
                    break;
                }
            }
            right_pos++;
        }
        std::string movie_name, Composer_rank;
        movie_name.assign(input_line, left_pos, right_pos - left_pos + 1);
        insertDB(Composer_name, movie_name);
    }
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
            splitMoiveName(0, Composer_name_, input_line);
        }            // This line is Composers name + movie name
        else {
            Composer_name_ = std::move(splitComposersName(input_line));
        }
    }
}

