#ifndef _HTTP_HANDLER_HPP_
#define _HTTP_HANDLER_HPP_
#include <string>
#include <unordered_map>


namespace imdb{

using namespace std;

class MovieDB;

class HttpHandler
{
    static MovieDB *db_;

    static int proc_query(const unordered_map<string, string>& params, string& ret, string& ret_type);

    static int proc_movie(const unordered_map<string, string>& params, string& ret, string& ret_type);

    static int proc_people(const unordered_map<string, string>& params, string& ret, string& ret_type);

    static int proc_main(const unordered_map<string, string>& params, string& ret, string& ret_type);

    static int proc_ico(const unordered_map<string, string>& params, string& ret, string& ret_type);
    //Handler is a function pointer, it is the type of proc_query and so on
    typedef int (*Handler)(const unordered_map<string, string>& params, string& ret, string& ret_type);
    
    static unordered_map<string, Handler> handlers_;
    static unordered_map<string, string> html_templates_;

public:

    static void Init(MovieDB *db);
    static void LoadTemplates(const string& dir);

    //process("/search", "[type = movie] [title = 'edge of tomorrow']", 
    //ret = "<html>...</html>", ret_type = "text/html"
    static int process(const string& path, const unordered_map<string, string>& params, string& ret, string& ret_type);


};




}

#endif

