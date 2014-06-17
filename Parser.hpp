#ifndef _PARSER_HPP_
#define _PARSER_HPP_

#include <iostream>
#include <string>
#include <unordered_map>
#include <fstream>
#include <cassert>
#include <vector>

// base class for the parser

class Parser {
public:

    Parser(const std::string str) : file_name_(str), line_num_(0) {
    }
    void parseFile(const std::string file_name);
    virtual void parseLine(const std::string line) = 0;
    std::string file_name_;
    long long line_num_;
};

class ActorsParser : public Parser {
public:

    ActorsParser(const std::string str) : Parser(str), begin_parse_(false) {
    }
    std::vector<std::string> splitMoiveName(const size_t begin, const std::string& input_line);
    std::string splitActorsName(const std::string& input_line);
    virtual void parseLine(const std::string line);
private:
    bool begin_parse_;
};

class ActressesParser : public Parser {
public:

    ActressesParser(const std::string str) : Parser(str) {
    };

    virtual void parseLine(const std::string line) {
    }
};

class KeywordsParser : public Parser {
public:

    KeywordsParser(const std::string str) : Parser(str) {
    };

    virtual void parseLine(const std::string line) {

    }
private:
    std::string file_name;
};

class DirectorsParser : public Parser {
public:

    DirectorsParser(const std::string str) : Parser(str) {
    };

    virtual void parseLine(const std::string line) {

    }
};

class TypeTable {
private:
    // map of data filename relate to Parser
    std::unordered_map<std::string, Parser*> parser_map;

public:
    TypeTable() = default;
    void insert(const std::string file_name);
    void exec(const std::string file_name);
};

#endif
