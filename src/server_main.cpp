#include <common/MovieDB.hpp>
#include <server/http_server.hpp>
#include <server/HttpHandler.hpp>

#include <unordered_map>
#include <string>
#include <cstdio>
#include <cstdlib>

using namespace std;
using namespace imdb;
using namespace tws;

MovieDB *mdb = nullptr;

#include <iostream>

/* Al%2B%2503+Pacino  =>  Al Pacino */
static string url_escape_remove(const string& s) {
    string ret;
    for (size_t i = 0; i < s.size(); i++)
        if (s[i] == '%' && i + 2 < s.size()) {
            auto hextodec = [](char c) {
                c = tolower(c);
                if (c >= '0' && c <= '9')
                    return c - '0';
                else
                    return c - 'a' + 10;
            };
            ret.append(1, hextodec(s[i + 1]) * 16 + hextodec(s[i + 2]));
            i += 2;
        } else if (s[i] == '+')
            ret.append(1, ' ');
        else
            ret.append(1, s[i]);
    return move(ret);
}

// http://127.0.0.1:8964/asdf?query_type=movie&movie_title=Al%2B%2503+Pacino
// split out   /asdf,   query_type => movie,   movie_title => al pacino 

static void parse_request(const string& s, string& path, unordered_map<string, string>& params) {
    auto path_param = split_string(s, "?", 2);
    path = path_param[0];
    if (path_param.size() == 1)
        return;

    auto param_pairs = split_string(path_param[1], "&");
    for (const auto& i : param_pairs) {
        auto key_value = split_string(i, "=", 2);
        if (key_value.size() == 2) {
            params[url_escape_remove(key_value[0])] = url_escape_remove(key_value[1]);
        }
    }
}

static int my_handler(Response& resp, const Request& req) {
    if (req.type() != HTTP_GET)
        return HTTP_404;

    string path;
    unordered_map<string, string> params;

    parse_request(req.path(), path, params);

    cout << "REQ path:" << path << endl;
    for (const auto& it : params)
        cout << it.first << " : " << it.second << endl;

    string ret, ret_type;
    if (HttpHandler::process(path, params, ret, ret_type) == 0) {
        resp.set_body(ret);
        resp.set_header("Content-Type", ret_type);
        return HTTP_200;
    }
    return HTTP_404;
}

int main(int argc, char *argv[]) {
    int port = 8000;

    if (argc < 4)
        printf("Usage: %s port db.filename html_template_path\n", argv[0]);

    port = atoi(argv[1]);

    mdb = new MovieDB();
    mdb->LoadFromFile(argv[2]);

    mdb->BuildIndex();

    // should catch execeptions if not sure the port is valid
    HttpHandler::Init(mdb);
    HttpHandler::LoadTemplates(argv[3]);
    tws::HttpServer http_server(port, &my_handler, 4);
    http_server.run();
    return 0;
}


