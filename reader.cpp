#include "reader.hpp"


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

void TypeTable::call_parseLine(const std::string file_name) {
	parser_map[file_name] -> parseLine();
}

int main(int argc, char *argv[]) {
	TypeTable testType;
	testType.insert("actors.list");
	testType.call_parseLine("actors.list");
	return 0;
} 