#include <HttpHandler.hpp>
#include <MovieDB.hpp>
#include <Log.hpp>

#include <fstream>



namespace imdb{


MovieDB* HttpHandler::db_ = nullptr;
string HttpHandler::image_ico_;
unordered_map<string, HttpHandler::Handler> HttpHandler::handlers_;


void HttpHandler::Init(MovieDB *db, const string& ico)
{
    db_ = db;
#define AH(a, b) handlers_.insert(make_pair(a, &b));
    AH("/query", HttpHandler::proc_query);
    AH("/movie", HttpHandler::proc_movie);
    AH("/people", HttpHandler::proc_people);
    AH("/favicon.ico", HttpHandler::proc_ico);
    AH("/", HttpHandler::proc_main);
#undef AH

     ifstream ifs(ico);
     if (ifs.good()) {
        image_ico_.assign( (std::istreambuf_iterator<char>(ifs) ),
        (std::istreambuf_iterator<char>()    ));
        LOG_INFO("Lode ICON succeed, size %llu", image_ico_.size());
     } else {
        LOG_DEBUG("Lode ICON[%s] failed", ico.c_str());
     }
     ifs.close();
}

int HttpHandler::process(const string& path, const unordered_map<string, string>& params, string& ret, string& ret_type)
{
    auto handler = handlers_.find(path);
    if (handler == handlers_.end()) {
        LOG_DEBUG("unknow handler for [%s]", path.c_str());
        return -1;
    }
    return (handler->second)(params, ret, ret_type);
}

int HttpHandler::proc_query(const unordered_map<string, string>& params, string& ret, string& ret_type)
{
    ret = "<html><body><h1>Query Page</h1></body></html>";
    ret_type = "text/html";
    return 0;
}


int HttpHandler::proc_movie(const unordered_map<string, string>& params, string& ret, string& ret_type)
{
    ret = "<html><body><h1>Movie Page</h1></body></html>";
    ret_type = "text/html";
    return 0;
}

int HttpHandler::proc_people(const unordered_map<string, string>& params, string& ret, string& ret_type)
{
    ret = "<html><body><h1>People Page</h1></body></html>";
    ret_type = "text/html";
    return 0;
}

int HttpHandler::proc_main(const unordered_map<string, string>& params, string& ret, string& ret_type)
{
    ret = "<html><body><h1>Main Page</h1></body></html>";
    ret_type = "text/html";
    return 0;
}

int HttpHandler::proc_ico(const unordered_map<string, string>& params, string& ret, string& ret_type)
{
    if (!image_ico_.empty()) {
        ret = image_ico_;
        ret_type = "image/jpeg";
        return 0;
    }
    return -1;
}

}

