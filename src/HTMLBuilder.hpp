#include <string>
#include <vector>

class HTMLBuilder {
public:
    HTMLBuilder();
    void Init();

    void outPut(const std::string content);
    void buildHref(const std::string content);

    void writeMoiveTitle(const std::string moiveTitle);

    void writeActors(std::vector<std::string> actors_arr);

    void writeHead();
    void writeContent();
    void writeFooter();

    std::string output_buffer_;

private:
    std::string file_name_;
};
