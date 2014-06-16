#include "reader.hpp"

// load the database to the reader
void Reader::openfile(const std::string file_name) {
    std::string line;
    std::ifstream inputfile (file_name);
    if (inputfile.is_open()) {
        while (getline(inputfile, line)) {
            parseLine(line);
        }
        inputfile.close();
    }
}

// should delete the object
void TypeTable::insert(const std::string file_name) {
    
	if (file_name == "actors.list") {
        parser_map[file_name] = new ActorsReader(file_name);
    }
    
    else if (file_name == "actresses.list") {
    	parser_map[file_name] = new ActressesReader(file_name);
    }
    
    else if (file_name == "keywords.list") {
    	parser_map[file_name] = new KeywordsReader(file_name);
    }
    
    else if (file_name == "directors.list") {
    	parser_map[file_name] = new DirectorsReader(file_name);
    }
}

void TypeTable::exec(const std::string file_name) {
    assert(parser_map.count(file_name));
    parser_map[file_name] -> openfile(file_name);
}

int main(int argc, char *argv[]) {
	TypeTable testType;
	testType.insert("actors.list");
	testType.exec("actors.list");
	return 0;
} 