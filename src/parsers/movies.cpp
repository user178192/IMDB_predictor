#include <Parser.hpp>
#include <Common.hpp>
#include <MovieDB.hpp>
#include <Log.hpp>
#include <cstring>
#include <algorithm>

std::string get_vaild_subtitle(const std::string input_line, const size_t begin) {
    auto left_pos = input_line.find('{', begin);
    if (left_pos != std::string::npos && input_line[left_pos + 1] != '{') {
        size_t right_pos = input_line.find('}', left_pos + 1);
        return string(input_line, left_pos + 1, right_pos - left_pos - 1);
    }
    return "";
}

std::string get_vaild_type(const std::string input_line, const size_t begin) {
    auto checker = input_line.find('{', begin);
    auto left_pos = input_line.find('(', begin);
    if (checker != std::string::npos &&
            left_pos != std::string::npos &&
            left_pos > checker) { // means that the {......( invaild
        return "";
    }

    if (left_pos != std::string::npos) {
        size_t right_pos = input_line.find(')', left_pos + 1);
        std::string temp(input_line, left_pos + 1, right_pos - left_pos - 1);
        if (temp.find_first_not_of("TV") == std::string::npos && temp.length() < 3) {
            return temp;
        }
    }
    return "";
}

void MoviesParser::Init() {
    title_ = subtitle_
            = type_
            = year_ = "";
}

void MoviesParser::Finish() {
    LOG_INFO("Read in %llu movies", db_->movies_.Size());
}

void MoviesParser::splitMoiveName(const std::string input_line) {
    // split by tab
    auto split_vec = split(input_line, '\t');
    // split_vec[0] is the title + subtitle
    auto title = split_vec[0];

    year_.assign(split_vec.back());

    size_t end = find_year_pos(title, 0);
    title_.assign(title, 0, end + 1);

    if (title_[0] == '\"') {
        // For tv title , remove the "
        title_.erase(std::remove(title_.begin(), title_.end(), '\"'), title_.end());
    }

    type_ = get_vaild_type(input_line, end);

    subtitle_ = get_vaild_subtitle(title, end);


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
        } else {
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