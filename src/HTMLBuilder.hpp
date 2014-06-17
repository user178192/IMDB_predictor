#include <string>
#include <vector>

class HTMLBuilder
{
public:
	HTMLBuilder();

	void outputFile(const std::string content);
	void buildHref(const std::string content);
	vector<string> regex(const std::string content);

	
	void writeTitile();
	void writeHead();
	void writeContent();
	void writeFooter();

	~HTMLBuilder();

private:
	std::string file_name_;

};
