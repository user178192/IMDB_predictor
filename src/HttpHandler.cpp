#include <HttpHandler.hpp>
#include <MovieDB.hpp>
#include <Log.hpp>



namespace imdb{


MovieDB* HttpHandler::db_ = nullptr;
unordered_map<string, HttpHandler::Handler> HttpHandler::handlers_;


void HttpHandler::Init(MovieDB *db)
{
    db_ = db;
#define AH(a, b) handlers_.insert(make_pair(a, &b));
    AH("/query", HttpHandler::proc_query);
    AH("/movie", HttpHandler::proc_movie);
    AH("/people", HttpHandler::proc_people);
    AH("/", HttpHandler::proc_main);
#undef AH
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

}

