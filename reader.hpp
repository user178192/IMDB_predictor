#include <iostream>
#include <string>
#include <unordered_map>
#include <stdexcept>

class Reader {
public: 
	Reader(const std::string str):file_name(str){}
	void openfile(std::string file_name);
	virtual void parseLine() = 0;

private:
	std::string file_name;
};

class ActorsReader : public Reader {
public:
	ActorsReader(const std::string str) : Reader(str){};
	virtual void parseLine() {
		std::cout << "in ActorsReader" << std::endl;
	};
};

class ActressesReader : public Reader {
public:
	ActressesReader(const std::string str) : Reader(str){};
	virtual void parseLine() {
		std::cout << "in ActressesReader" << std::endl;
	}
};

class KeywordsReader : public Reader {
public:
	KeywordsReader(const std::string str) : Reader(str){};
	virtual void parseLine() {

	}
private:
	std::string file_name;
};

class DirectorsReader : public Reader {
public:
	DirectorsReader(const std::string str) : Reader(str){};
	virtual void parseLine() {

	}
};


class TypeTable {
    private:
        // map of data filename relate to Reader 
        std::unordered_map<std::string, Reader*> parser_map;

    public:
        TypeTable() = default;
        void insert(const std::string file_name);
        void call_parseLine(const std::string file_name);
};

