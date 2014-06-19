#include "Parser.hpp"
#include <MovieDB.hpp>
#include <Log.hpp>

void MoviesParser::Init() {
	title_ = subtitle_ 
           = type_ 
           = year_ = "";
}

void MoviesParser::Finish() {
	LOG_INFO("Read in %llu movies", db_->movies_.Size());
}

void MoviesParser::splitMoiveName(const std::string input_line) { 
	size_t left_pos = 0;
	size_t right_pos = input_line.length(); 
	left_pos = input_line.rfind('\t', right_pos - 1);
	year_.assign(input_line, left_pos + 1, right_pos - left_pos - 1);

	// skip tab
	while (input_line[left_pos] == '\t') {left_pos--;}

	while(left_pos != 0) {
		if (input_line[left_pos] == '}') {
		// here is subtitle
			right_pos = left_pos; 
			left_pos = input_line.rfind('{', right_pos - 1);
			subtitle_.assign(input_line, left_pos + 1, right_pos - left_pos - 1);
		} 
		else if(input_line[left_pos] == ')') {
		    // here is the year in title;
		    // special case like (1999/II) 
		    size_t close = input_line.rfind('(', left_pos);
		    right_pos = left_pos;
			string temp(input_line, close + 1, right_pos - close - 1);
			// >=4 avoid like (V)
			if (temp.find_first_not_of("0123456789IVX/?") == string::npos && temp.length() >= 4) {
		     		left_pos = 0;

		     		title_.assign(input_line, left_pos, right_pos - left_pos + 1);

		     		if (title_[0] == '"') {
		     			// For tv title , remove the "
		     			title_.erase (std::remove(title_.begin(), title_.end(), '"'), title_.end());
			 		}
			 		break;
			}
			else {
					// here is the type
					left_pos = input_line.rfind('(', right_pos);
					type_.assign(input_line, left_pos + 1, right_pos - left_pos - 1);
			}
		}
		left_pos--; 
	}

	//std::cout << "{" << title_ << "}" << year_ << std::endl; 
}

void MoviesParser::parseLine(const std::string input_line) {
	//refresh the string in evey line
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

void MoviesParser::insertDB() {
    string key = title_;
    auto mov_obj = db_->movies_.GetInfo(key);
    if (get<0>(mov_obj)) {
        // already exists, and have subtitle, (series)
        // insert subtitle
        if (!subtitle_.empty()) {
            get<2>(mov_obj)->subtitles_.push_back(subtitle_);
        }
    } 
    else {
        // insert new entry
        Movie m;
        m.year_ = year_;
        m.type_ = type_;
        db_->movies_.Insert(key, m);
    }
}