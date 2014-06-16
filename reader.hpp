#include <iostream>
#include <string>
#include <unordered_map>
#include <fstream>
#include <cassert>

// base class for the parser
class Reader {
public:
	Reader(const std::string str):file_name(str){}
	void openfile(const std::string file_name);
	virtual void parseLine(std::string line) = 0;
    
private:
	std::string file_name;
};


class ActorsReader : public Reader {
public:
	ActorsReader(const std::string str) : Reader(str){}
	virtual void parseLine(std::string line) {
        std::cout << line << std::endl;
	}
};

class ActressesReader : public Reader {
public:
	ActressesReader(const std::string str) : Reader(str){};
	virtual void parseLine(std::string line) {
	}
};

class KeywordsReader : public Reader {
public:
	KeywordsReader(const std::string str) : Reader(str){};
	virtual void parseLine(std::string line) {
        
	}
private:
	std::string file_name;
};

class DirectorsReader : public Reader {
public:
	DirectorsReader(const std::string str) : Reader(str){};
	virtual void parseLine(std::string line) {
        
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

