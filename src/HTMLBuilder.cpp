#include "HTMLBuilder.hpp"

#include <iostream>
HTMLBuilder::HTMLBuilder() {

}

void HTMLBuilder::Init() {
	output_buffer_ = "";
}

void HTMLBuilder::writeMoiveTitle(const std::string moiveTitle) {
	outPut("<h2>" + moiveTitle + "</h2>");
}


void HTMLBuilder::writeActors(std::vector<std::string> actors_arr) {
	outPut("<li>");
	for (auto &str : actors_arr) {
		outPut("<a>" + str + "</a>" + ",");
	}
	outPut("</li>\n");
}

void HTMLBuilder::writeHead() {
	outPut("<!DOCTYPE html>\n");
	outPut("<html>\n");
	outPut("<head><title>IMDB Predictor</title>\n");
	outPut("<body>\n");
	outPut("<h1>IMDB Predictor</h1>\n");
}

void HTMLBuilder::buildHref(const std::string content) {

}

void HTMLBuilder::writeContent() {
	
}

void HTMLBuilder::writeFooter() {
    outPut("<body>\n");
	outPut("</html>\n");
}


void HTMLBuilder::outPut(const std::string content) {
	  output_buffer_ += content;
}

int main(int argc, char const *argv[])
{
	HTMLBuilder test;
	test.Init();
	test.writeHead();
	test.writeMoiveTitle("Taxi Driver (1976)");
	test.writeActors({"Robert De Niro", "Judi Foster"});
	test.writeFooter();
	std::cout << test.output_buffer_;

	return 0;
}