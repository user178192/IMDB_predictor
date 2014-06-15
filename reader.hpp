#include <iostream>
#include <string>
#include <unordered_map>

class Reader {
public: 
	Reader(std::string str):file_name(str){};
	virtual void parser(std::string file_name) = 0;

private:
	std::string file_name;
};

class ActorsReader : public Reader {
public:
	virtual void parser(std::string file_name);
};

class ActressesReader : public Reader {
public:
	virtual void parser(std::string file_name);
};

class KeywordsReader : public Reader {
public:
	virtual void parser(std::string file_name);
};

class DirectorsReader : public Reader {
public:
	virtual void parser(std::string file_name);
};


class TypeTable {
    private:
        // map of data filename relate to Reader 
        std::unordered_map<std::string, std::unique_ptr<Reader>> parser_map;

    public:
        TypeTable() = default;
};

