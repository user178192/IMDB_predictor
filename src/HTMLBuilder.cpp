#include "HTMLBuilder.hpp"

#include <fstream>
HTMLBuilder::HTMLBuilder() {

}


vector<string> HTMLBuilder::regex(const std::string content) {

}

HTMLBuilder::writeHead() {
	outputFile("<!DOCTYPE html>");
	outputFile("<html>");
	outputFile("<head><title>IMDB Predictor</title>");
	outputFile("<body>");
	outputFile("<h1>IMDB Predictor</h1>");
}

HTMLBuilder::buildHref() {

}

HTMLBuilder::writeContent() {
	
}

HTMLBuilder::writeFooter() {
    outputFile("<body>");
	outputFile("</html>");
}


void HTMLBuilder::outputFile(const std::string content) {
	fstream fout;
	fout.open(file_name_, std::fstream::app);
	fout << content + "\n";
	fout.close();
}