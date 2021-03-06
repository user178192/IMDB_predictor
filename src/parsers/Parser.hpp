#ifndef _PARSER_HPP_
#define _PARSER_HPP_

#include <iostream>
#include <string>
#include <unordered_map>
#include <fstream>
#include <cassert>
#include <vector>
#include <algorithm>


// debug output
template <typename T>
auto debug(T t) -> decltype(t) {
    std::cout << t << std::endl;
    return t;
}

// base class for the parser
namespace imdb {
    class MovieDB;
}

using namespace imdb;

class Parser {
protected:
    MovieDB *db_;

    virtual void Init() {
    }

    virtual void Finish() {
    }

public:

    Parser(const std::string str, MovieDB *db) : db_(db), file_name_(str), line_num_(0) {
        Init();
    }

    virtual ~Parser() {
        
    }

    void parseFile(const std::string file_name);
    virtual void parseLine(const std::string line) = 0;
    std::string file_name_;
    long line_num_;
};

class ActorsParser : public Parser {
public:
    using Parser::Parser;
    virtual void Init();
    virtual void Finish();
    virtual void parseLine(const std::string line);

    void splitMoiveName(const size_t begin, const std::string& actor_name, const std::string& input_line);
    std::string splitActorsName(const std::string& input_line);
    void insertDB(const std::string& actor_name, const std::string& movie_name, const std::string& actor_rank);

private:
    bool begin_parse_ = false;
    std::string actor_name_;
};

class DirectorsParser : public Parser {
public:
    using Parser::Parser;
    virtual void Init();
    virtual void parseLine(const std::string line);
    virtual void Finish();

    void splitMoiveName(const size_t begin, const std::string& actor_name, const std::string& input_line);
    std::string splitDirectorsName(const std::string& input_line);
    void insertDB(const std::string& actor_name, const std::string& movie_name);

private:
    bool begin_parse_ = false;
    std::string director_name_;
};

class ComposersParser : public Parser {
public:
    using Parser::Parser;
    virtual void Init();
    virtual void parseLine(const std::string line);
    virtual void Finish();

    void splitMoiveName(const size_t begin, const std::string& composers_name, const std::string& input_line);
    std::string splitComposersName(const std::string& input_line);
    void insertDB(const std::string& actor_name, const std::string& movie_name);

private:
    bool begin_parse_ = false;
    std::string composer_name_;
};

class MoviesParser : public Parser {
public:
    using Parser::Parser;
    virtual void Init();
    virtual void parseLine(const std::string line);
    virtual void Finish();

    void splitMoiveName(const std::string input_line);
    void insertDB();

private:
    bool begin_parse_ = false;
    std::string title_, subtitle_, type_, year_;
};

class GenresParser : public Parser {
public:
    using Parser::Parser;

    virtual void Init();
    virtual void parseLine(const std::string line);
    virtual void Finish();

    void splitMoiveName(const std::string input_line);
    void insertDB();

private:
    bool begin_parse_ = false;
    std::string title_, genr_;
};

class LanguagesParser : public Parser {
public:
    using Parser::Parser;
    virtual void Init();
    virtual void parseLine(const std::string line);
    virtual void Finish();

    void splitMoiveName(const std::string input_line);
    void insertDB();    
private:
    bool begin_parse_ = false;
    std::string title_, lang_;
};

class CountriesParser : public Parser {
public:
    using Parser::Parser;
    virtual void Init();
    virtual void parseLine(const std::string line);
    virtual void Finish();

    void splitMoiveName(const std::string input_line);
    void insertDB();
private:
    bool begin_parse_ = false;
    std::string title_, country_;
};

class RunningtimeParser : public Parser {
public:
    using Parser::Parser;
    virtual void Init();
    virtual void parseLine(const std::string line);
    virtual void Finish();

    void splitMoiveName(const std::string input_line);
    void insertDB();

private:
    bool begin_parse_ = false;
    std::string title_, length_;
};

class RatingParser : public Parser
{
public:
	using Parser::Parser;

    virtual void Finish();
	virtual void parseLine(const std::string line);
    void insertDB();
private:
   std::string title_;
   double rating_;
   size_t votes_; 
};




// should delete the object in the end

class TypeTable {
private:
    // map of data filename relate to Parser
    std::unordered_map<std::string, Parser*> parser_map;
public:
    TypeTable() = default;
    void init(MovieDB *db);
    void exec(const std::string file_name);

    // insert the realtion movies 
    void join(MovieDB *db);
};

#endif
