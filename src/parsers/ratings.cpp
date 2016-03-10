#include <parsers/Parser.hpp>
#include <common/Log.hpp>
#include <common/MovieDB.hpp>
#include <stdio.h>


using namespace imdb;


static bool is_year_tag(const char *s)
{
    if (*s-- != ')') return false;
    const char *t = s;
    // safe check ignored
    while(*t != '(')
        t--;
    t++;

    if (!isdigit(*t++)) return false;
    if (!isdigit(*t++)) return false;
    if (!isdigit(*t++)) return false;
    if (!isdigit(*t++)) return false;
    return true;
}

void RatingParser::Finish() 
{
    LOG_INFO("Finish");
}

void RatingParser::parseLine(const std::string input_line) 
{
    if (!isdigit(input_line[6]) && input_line[6] != '.')
        return;

    int pos = 16;
    while(input_line[pos] == ' ')
        pos++;
    
    votes_ = (size_t)atol(input_line.c_str() + pos);
    while(isdigit(input_line[pos]))
        pos++;

    while(input_line[pos] == ' ')
        pos++;

    rating_ = atof(input_line.c_str() + pos);
    while(isdigit(input_line[pos]) || input_line[pos] == '.' || input_line[pos] == '\"')
        pos++;
    while(input_line[pos] == ' ' || input_line[pos] == '\"')
        pos++;
    
    string key;
    for(; input_line[pos] != '\0'; pos++) {
        if (input_line[pos] == '\"' && input_line[pos + 1] == ' '
                && input_line[pos + 2] == '(')
            continue;
        key.append(1, input_line[pos]);
    }

    size_t ksize = key.size();
    int sstack = 0;
    for(size_t i = key.size() - 1; i > 5; i--) {
        if (key[i] == '}')
            sstack++;
        else if (key[i] == '{') {
            sstack--;
        }

        if (sstack == 0) {
            if (is_year_tag(&key[i]))
                ksize = i + 1;
        }
    }

    title_ = key.substr(0, ksize);

    if (title_.empty()) {
        //no title
        return;
    } else {
        insertDB();
    }
}

void RatingParser::insertDB() {
    string key = title_;
    auto mov_obj = db_->movies_.GetInfo(key);
    if (get<0>(mov_obj)) {
        // already exists
        // skip this movie
        LOG_DEBUG("Movie [%s] already exits", key.c_str());
    }
    else {
        // the movie doesnt exist in db
        // if votes larger than 10, then insert new entry
        if (votes_ > 50 && rating_ > 4.0) {
            LOG_DEBUG("Movie [%s] doesnt exits and votes larger than 50 and rate lager than 4.0", key.c_str());
            Movie m;
            m.rating_ = rating_;
            m.votes_ = votes_;
            db_->movies_.Insert(key, m);
        }
    }
}
