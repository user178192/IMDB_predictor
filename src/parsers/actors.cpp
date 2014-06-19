#include <Parser.hpp>
#include <Log.hpp>
#include <MovieDB.hpp>
#include <stdio.h>

#include <cstring>
#include <climits>

using namespace imdb;

void ActorsParser::Init() {

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

void ActorsParser::Finish() {
    LOG_INFO("Read in %llu actors", db_->actors_.Size());
}

void ActorsParser::insertDB(const std::string& actor_name, const std::string& movie_name, const std::string& actor_rank) {
    auto act_key = actor_name;
    auto mov_key = movie_name;
    size_t actor_id = 0;

    auto act_obj = db_ -> actors_.GetInfo(act_key); // here is the actors info
    auto mov_obj = db_ -> movies_.GetInfo(mov_key);

    if (!get<0>(mov_obj)) {
        LOG_DEBUG("Movie [%s] not found, inconsistant", movie_name.c_str());
        return;
    }

    // if no rank, set INT_MAX
    int rank = actor_rank.length() == 0 ? INT_MAX : std::stoi(actor_rank);

    if (get<0>(act_obj)) {
        // the actor already exists, insert the movie id to the actor
        get<2>(act_obj)->movies_.push_back(get<1>(mov_obj));
        get<2>(mov_obj)->AddActor(actor_id, rank);

    } else {
        // insert new actor
        Actor a;
        a.movies_.push_back(get<1>(mov_obj));
        actor_id = db_->actors_.Insert(act_key, a);
        get<2>(mov_obj)->AddActor(actor_id, rank);
    }
}

/*
    For Television Series 
    Not consider the subtitle
*/

void ActorsParser::splitMoiveName(const size_t begin, const std::string& actor_name, const std::string& input_line) {
    size_t left_pos = begin;

    while (input_line[left_pos] == '\t') {
        left_pos++;
    }

    // it is a Television Series 
    if (input_line[left_pos] == '"') {

        std::string series_name, series_time, actor_rank;
        size_t right_pos = input_line.find('"', left_pos + 1);


        series_name.assign(input_line, left_pos + 1, right_pos - left_pos - 1);

        left_pos = input_line.find('(', right_pos + 1);
        if (left_pos != std::string::npos) {
            right_pos = input_line.find(')', left_pos + 1);
            series_time.assign(input_line, left_pos + 1, right_pos - left_pos - 1);
        }

        left_pos = input_line.find('<', right_pos + 1);
        if (left_pos != std::string::npos) {
            right_pos = input_line.find('>', left_pos + 1);
            actor_rank.assign(input_line, left_pos + 1, right_pos - left_pos - 1);
        }

        insertDB(actor_name, series_name + " " + "(" + series_time + ")", actor_rank);
    }
        // it is a Movie
    else {

        size_t right_pos = 0;
        while (right_pos < input_line.length() ) { // find the end of year   
            if (input_line[right_pos] == ')') {
                //special case like (1999/II)
                size_t close = input_line.rfind('(', right_pos);
                if (isdigit(input_line[close + 1]) || input_line[close + 1] == '?') {
                    break;
                }
            }
            right_pos++;
        }

        std::string movie_name, actor_rank;
        movie_name.assign(input_line, left_pos, right_pos - left_pos + 1);

        left_pos = input_line.find('<', right_pos + 1);
        if (left_pos != std::string::npos) {
            right_pos = input_line.find('>', left_pos + 1);
            actor_rank.assign(input_line, left_pos + 1, right_pos - left_pos - 1);
        }

        insertDB(actor_name, movie_name, actor_rank);
    }
}

void ActorsParser::parseLine(const std::string input_line) {
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
            splitMoiveName(0, actor_name_, input_line);
        }            // This line is actors name + movie name
        else {
            actor_name_ = std::move(splitActorsName(input_line));
        }
    }
}

