#ifndef _PARSER_HPP_
#define _PARSER_HPP_

#include <iostream>
#include <string>
#include <unordered_map>
#include <fstream>
#include <cassert>
#include <vector>

// base class for the parser
namespace imdb {
    class MovieDB;
}

using namespace imdb;

class Parser {
protected:
    MovieDB *db_;
    virtual void Init() {}
    
public:
    Parser(const std::string str, MovieDB *db) : db_(db), file_name_(str), line_num_(0) {
        Init();
    }
    void parseFile(const std::string file_name);
    virtual void parseLine(const std::string line) = 0;
    std::string file_name_;
    long long line_num_;
};

class ActorsParser : public Parser {
public:
    using Parser::Parser;
    virtual void Init();
    void splitMoiveName(const size_t begin, const std::string& actor_name, const std::string& input_line);
    std::string splitActorsName(const std::string& input_line);
    virtual void parseLine(const std::string line);

private:
    bool begin_parse_;
    std::string actor_name_, series_name1_, series_name2_, series_time_, actor_rank_;
    std::string movie_name_, movie_time_;
};

class ActressesParser : public Parser {
public:
    using Parser::Parser;

    virtual void Init() {}

    virtual void parseLine(const std::string line) {
    }
};

class KeywordsParser : public Parser {
public:
    using Parser::Parser;

    virtual void Init() {}

    virtual void parseLine(const std::string line) {

    }
private:
    std::string file_name;
};

class DirectorsParser : public Parser {
public:
    using Parser::Parser;

    virtual void Init() {}

    virtual void parseLine(const std::string line) {

    }
};

class  MoviesParser : public Parser
{
public:
    using Parser::Parser;

    virtual void Init();

    virtual void parseLine(const std::string line);
private:
	std::string title_, titleyear_, subtitle_;
	std::string movietype_, year_;
};
#endif
