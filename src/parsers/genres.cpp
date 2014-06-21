#include "Parser.hpp"
#include <MovieDB.hpp>
#include <Log.hpp>

using namespace imdb;

void GenresParser::Init() {
	title_ = genr_ = "";
}

void GenresParser::Finish() {
	LOG_INFO("Read in %llu movies", db_->movies_.Size());
}

void GenresParser::splitMoiveName(const std::string input_line) { 
	if (strncmp(input_line.c_str(), "=====", 5) == 0) {
        return;
    }


    auto split_vec = split(input_line, '\t');
    // split_vec[0] is the title + subtitle
    auto title = split_vec[0];

	genr_.assign(split_vec.back());

    if (title[0] == '\"') {
         // For tv title , remove the "
         title.erase (std::remove(title.begin(), title.end(), '\"'), title.end());
    }

    size_t end = find_year_pos(title, 0);
	title_.assign(title, 0, end + 1);
}

void GenresParser::parseLine(const std::string input_line) {
	Init();
	if (strncmp(input_line.c_str(), "8: THE GENRES LIST", 18) == 0) {
        begin_parse_ = true;
        return;
    }

	if (begin_parse_ == true) { 
		// This line is empty line
		if (input_line.length() == 0) {
            return;
        }
        else {
        	splitMoiveName(input_line);
        	//insertDB();
        }
    }
}

void GenresParser::insertDB() {
    string key = title_;
    if (key.empty()) {return; }
    auto mov_obj = db_->movies_.GetInfo(key);
    if (get<0>(mov_obj)) {
    // insert Genres, since one movie may have mutiple Genres
    	if (!genr_.empty()) {
        	get<2>(mov_obj)->genres_.push_back(genr_);
    	}
    } else {
        // no such movie, inconsistant, ignore
        LOG_DEBUG("Movie [%s] not found, inconsistant", key.c_str());
    };
}
