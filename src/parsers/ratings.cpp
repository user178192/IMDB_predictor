#include <Parser.hpp>
#include <Log.hpp>
#include <MovieDB.hpp>
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
    
    size_t votes = (size_t)atol(input_line.c_str() + pos);
    while(isdigit(input_line[pos]))
        pos++;

    while(input_line[pos] == ' ')
        pos++;

    double rating = atof(input_line.c_str() + pos);
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

    key = key.substr(0, ksize);
    //cout << '[' << votes << "]  [" << rating << "]   [" << key << "]\n";

    auto db_ret = db_->movies_.GetInfo(key);
    if (get<0>(db_ret)) {
        get<2>(db_ret)->votes_ = votes;
        get<2>(db_ret)->rating_ = rating;
    } else {
        // no such movie, inconsistant, ignore
        LOG_DEBUG("Movie [%s] not found, inconsistant", key.c_str());
    }  

}


