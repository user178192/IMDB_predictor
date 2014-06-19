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
    static string image_ico_;

    static int proc_query(const unordered_map<string, string>& params, string& ret, string& ret_type);

    static int proc_movie(const unordered_map<string, string>& params, string& ret, string& ret_type);

    static int proc_people(const unordered_map<string, string>& params, string& ret, string& ret_type);

    static int proc_main(const unordered_map<string, string>& params, string& ret, string& ret_type);

    static int proc_ico(const unordered_map<string, string>& params, string& ret, string& ret_type);

    typedef int (*Handler)(const unordered_map<string, string>& params, string& ret, string& ret_type);
    
    static unordered_map<string, Handler> handlers_;

public:

    static void Init(MovieDB *db, const string& ico);

    //process("/search", "[type = movie] [title = 'edge of tomorrow']", 
    //ret = "<html>...</html>", ret_type = "text/html"
    static int process(const string& path, const unordered_map<string, string>& params, string& ret, string& ret_type);


};




}

#endif

