#include <iostream>
#include <string>
#include <unordered_map>
#include <fstream>
#include <cassert>
#include <vector>

// base class for the parser
class Reader {
public:
	Reader(const std::string str):file_name_(str),line_num_(0){}
	void parseFile(const std::string file_name);
	virtual void parseLine(const std::string line) = 0;
	std::string file_name_;
	long long line_num_;
};


class ActorsReader : public Reader {
public:
	ActorsReader(const std::string str) : Reader(str),begin_parse_(false){}
	std::vector<std::string> splitMoiveName(const size_t begin, const std::string& input_line);
    std::string splitActorsName(const std::string& input_line);
	virtual void parseLine(const std::string line);
private:
    bool begin_parse_;
};

class ActressesReader : public Reader {
public:
	ActressesReader(const std::string str) : Reader(str){};
	virtual void parseLine(const std::string line) {
	}
};

class KeywordsReader : public Reader {
public:
	KeywordsReader(const std::string str) : Reader(str){};
	virtual void parseLine(const std::string line) {
        
	}
private:
	std::string file_name;
};

class DirectorsReader : public Reader {
public:
	DirectorsReader(const std::string str) : Reader(str){};
	virtual void parseLine(const std::string line) {
        
	}
};


class TypeTable {
private:
    // map of data filename relate to Reader
    std::unordered_map<std::string, Reader*> parser_map;
    
public:
    TypeTable() = default;
    void insert(const std::string file_name);
    void exec(const std::string file_name);
};

