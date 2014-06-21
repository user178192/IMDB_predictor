#include <Parser.hpp>
#include <Common.hpp>
#include <MovieDB.hpp>
#include <Log.hpp>
#include <cstring>

using namespace imdb;

void LanguagesParser::Init() {
	title_ = lang_ = "";
}

void LanguagesParser::Finish() {
	LOG_INFO("Read in %llu movies", db_->movies_.Size());
}

void LanguagesParser::splitMoiveName(const std::string input_line) { 
    auto split_vec = split(input_line, '\t');
    // split_vec[0] is the title + subtitle
    auto title = split_vec[0];

	lang_.assign(split_vec.back());

    if (title[0] == '\"') {
         // For tv title , remove the "
         title.erase (std::remove(title.begin(), title.end(), '\"'), title.end());
    }
    size_t end = find_year_pos(title, 0);
	title_.assign(title, 0, end + 1);
}

void LanguagesParser::parseLine(const std::string input_line) {
	Init();
	if (strncmp(input_line.c_str(), "=====", 5) == 0) {
        begin_parse_ = true;
        return;
    }

    else if (strncmp(input_line.c_str(), "-----", 5) == 0) {
        begin_parse_ = false;
        return;
    }

	if (begin_parse_ == true) { 
		// This line is empty line
		if (input_line.length() == 0) {
            return;
        }
        else {
        	splitMoiveName(input_line);
        	insertDB();
        }
    }
}

void LanguagesParser::insertDB() {
    string key = title_;
    auto mov_obj = db_->movies_.GetInfo(key);
    if (get<0>(mov_obj)) {
    	// insert Languages, since one movie may have mutiple Languages
    	if (!lang_.empty()) {
        	get<2>(mov_obj)->languages_.push_back(lang_);
    	}
    } else {
        // no such movie, inconsistant, ignore
        LOG_DEBUG("Movie [%s] not found, inconsistant", key.c_str());
    }
}
