#include <server/HttpHandler.hpp>
#include <server/DynamicHTML.hpp>
#include <relation/MovieRelation.hpp>
#include <common/MovieDB.hpp>
#include <common/Log.hpp>

#include <fstream>
#include <algorithm>


using namespace dynamichtml;

namespace imdb{


MovieDB* HttpHandler::db_ = nullptr;
unordered_map<string, HttpHandler::Handler> HttpHandler::handlers_;
unordered_map<string, string> HttpHandler::html_templates_;


void HttpHandler::Init(MovieDB *db)
{
    db_ = db;
#define AH(a, b) handlers_.insert(make_pair(a, &b));
    AH("/query", HttpHandler::proc_query);
    AH("/movie", HttpHandler::proc_movie);
    AH("/people", HttpHandler::proc_people);
    AH("/favicon.ico", HttpHandler::proc_ico);
    AH("/", HttpHandler::proc_main);
#undef AH
}

void HttpHandler::LoadTemplates(const string& dir)
{
#define AH(a, b) html_templates_.insert(make_pair(a, b));
     AH("favicon", "favicon.jpg");
     AH("main", "main.html");
     AH("movie", "movie.html");
     AH("query", "query.html");
     AH("people", "people.html");
     AH("logo_and_searchbox", "logo_and_searchbox.tpl");
#undef AH

     for(auto &i : html_templates_) {
         char fn[128];
         snprintf(fn, 128, "%s/%s", dir.c_str(), i.second.c_str());
         ifstream ifs(fn);
         if (ifs.good()) {
            i.second.assign( (std::istreambuf_iterator<char>(ifs) ),
            (std::istreambuf_iterator<char>()    ));
            LOG_INFO("Load %s succeed, size %llu", fn, i.second.size());
         } else {
            LOG_DEBUG("Load file[%s] failed", fn);
         }
         ifs.close();
     }
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
    string template_html;
    {
        auto it = html_templates_.find("query");
        if (it == html_templates_.end())
            return -1;
        template_html = it->second;
    }

    string query_type, keywords;
    {
        auto it = params.find("query_type");
        if (it == params.end())
            return -1;
        query_type = it->second;

        it = params.find("keywords");
        if (it == params.end())
            return -1;
        keywords = it->second;
    }


    TemplateNode nodes;
    nodes.Insert("logo_and_searchbox", html_templates_["logo_and_searchbox"]);
    nodes.Insert("keywords", keywords);
    auto query_words = split_string(keywords, ", ()\t");
    auto query_result = db_->ri_movies_.Lookup(query_words);
    int ret_limit = 20;
    
    TemplateNode movielist;
    for(const auto& id : query_result) {
        if (ret_limit-- == 0)
            break;
        auto title = get<1>(db_->movies_.GetKey(id));
        TemplateNode tmp;
        char bufid[30];
        snprintf(bufid, 30, "%llu", id);
        tmp.Insert("id", bufid);
        tmp.Insert("title", *title);
        movielist.Insert(new TemplateNode(tmp));
    }
    nodes.Insert("result", new TemplateNode(movielist));
    ret.clear();
    vector<string> failed_tags;
    if (generate_html(template_html, nodes, ret, failed_tags) < 0) {
        LOG_ERROR("Html template applied failed:");
        for(const auto &i : failed_tags)
            LOG_ERROR("bad tag(s): %s", i.c_str());
        ret.clear();
        return -1;
    }
    ret_type = "text/html";
    return 0;
}


int HttpHandler::proc_movie(const unordered_map<string, string>& params, string& ret, string& ret_type)
{
    string template_html;
    {
        auto it = html_templates_.find("movie");
        if (it == html_templates_.end())
            return -1;
        template_html = it->second;
    }

    size_t id;
    {
        auto it = params.find("id");
        if (it == params.end())
            return -1;
        id = atol(it->second.c_str());
    }


    auto info = db_->movies_.GetInfo(id);
    Movie *m = get<2>(info);
    if (!get<0>(info))
        return -1;

    TemplateNode nodes, lists;
    nodes.Insert("logo_and_searchbox", html_templates_["logo_and_searchbox"]);

    nodes.Insert("movietitle", *get<1>(info));
    char tmpbuf[30];
    if (m->votes_ > 10) {
        snprintf(tmpbuf, 30, "%llu", m->votes_);
        nodes.Insert("votes", tmpbuf);
        snprintf(tmpbuf, 30, "%.1f", m->rating_);
        nodes.Insert("rating", tmpbuf);
    } else {
        nodes.Insert("votes", "< 10");
        nodes.Insert("rating", "Unavailable");
    }
    snprintf(tmpbuf, 30, "%.1f", m->rating_pred_);
    nodes.Insert("rating_pred", tmpbuf);

    lists.Clear();
    int actor_limit = 10;
    for(const auto &i : m->actors_) {
        if (actor_limit-- == 0)
            break;
        TemplateNode tmp;
        tmp.Insert("name", MovieDB::NameReorder(*(get<1>(db_->actors_.GetKey(i)))));
        snprintf(tmpbuf, 30, "%llu", i);
        tmp.Insert("id", tmpbuf);
        lists.Insert(new TemplateNode(tmp));
    }
    nodes.Insert("actors", new TemplateNode(lists));

    lists.Clear();
    for(const auto &i : m->directors_) {
        TemplateNode tmp;
        tmp.Insert("name", MovieDB::NameReorder(*(get<1>(db_->directors_.GetKey(i)))));
        snprintf(tmpbuf, 30, "%llu", i);
        tmp.Insert("id", tmpbuf);
        lists.Insert(new TemplateNode(tmp));
    }
    nodes.Insert("directors", new TemplateNode(lists));

    lists.Clear();
    if (m->composer_ != NULLID) {
        TemplateNode tmp;
        tmp.Insert("composername", MovieDB::NameReorder(*(get<1>(db_->composers_.GetKey(m->composer_)))));
        snprintf(tmpbuf, 30, "%llu", m->composer_);
        tmp.Insert("composerid", tmpbuf);
        lists.Insert(new TemplateNode(tmp));
    } 
    nodes.Insert("composers", new TemplateNode(lists));

    lists.Clear();
    for(const auto &i : m->genres_) {
        TemplateNode tmp;
        tmp.Insert("genre", i);
        lists.Insert(new TemplateNode(tmp));
    }
    nodes.Insert("genres", new TemplateNode(lists));

    lists.Clear();
    for(const auto &i : m->length_) {
        TemplateNode tmp;
        tmp.Insert("length", i);
        lists.Insert(new TemplateNode(tmp));
    }
    nodes.Insert("lengths", new TemplateNode(lists));

    lists.Clear();
    for(const auto &i : m->languages_) {
        TemplateNode tmp;
        tmp.Insert("language", i);
        lists.Insert(new TemplateNode(tmp));
    }
    nodes.Insert("languages", new TemplateNode(lists));

    lists.Clear();
    for(const auto &i : m->countries_) {
        TemplateNode tmp;
        tmp.Insert("country", i);
        lists.Insert(new TemplateNode(tmp));
    }
    nodes.Insert("countries", new TemplateNode(lists));

    lists.Clear();
    if (m->relation_.empty()) {
        //not pre-computed (votes not enough)
        m->relation_ = MovieRelation::GetSimilarMovies(*m, db_);
    }

    for(const auto &i : m->relation_) {
        TemplateNode tmp;
        tmp.Insert("name", *(get<1>(db_->movies_.GetKey(i))));
        snprintf(tmpbuf, 30, "%llu", i);
        tmp.Insert("id", tmpbuf);
        lists.Insert(new TemplateNode(tmp));
    }
    nodes.Insert("simis", new TemplateNode(lists));

    ret.clear();
    vector<string> failed_tags;
    if (generate_html(template_html, nodes, ret, failed_tags) < 0) {
        LOG_ERROR("Html template applied failed:");
        for(const auto &i : failed_tags)
            LOG_ERROR("bad tag(s): %s", i.c_str());
        ret.clear();
        return -1;
    }
    ret_type = "text/html";
    return 0;
}

int HttpHandler::proc_people(const unordered_map<string, string>& params, string& ret, string& ret_type)
{
    string template_html;
    {
        auto it = html_templates_.find("people");
        if (it == html_templates_.end())
            return -1;
        template_html = it->second;
    }

    size_t id;
    string query_type;
    {
        auto it = params.find("id");
        if (it == params.end())
            return -1;
        id = atol(it->second.c_str());

        it = params.find("type");
        if (it == params.end())
            return -1;
        query_type = it->second;
    }

    auto info = db_->actors_.GetInfo(0);
    if (query_type == "composer")
        info = db_->composers_.GetInfo(id);
    else if (query_type == "actor")
        info = db_->actors_.GetInfo(id);
    else if (query_type == "director")
        info = db_->directors_.GetInfo(id);
    else
        return -1;

    // actors composers directors are same type
    People *p = get<2>(info);
    if (!get<0>(info))
        return -1;

    TemplateNode nodes, lists;
    nodes.Insert("logo_and_searchbox", html_templates_["logo_and_searchbox"]);
    nodes.Insert("people", MovieDB::NameReorder(*(get<1>(info))));

    //sort the result
    auto movies = p->movies_;
    sort(movies.begin(), movies.end(), [&](const size_t a, const size_t b) {
                return get<2>(db_->movies_.GetInfo(a))->votes_ >
                get<2>(db_->movies_.GetInfo(b))->votes_;
            });
    for(const auto &i : movies) {
        auto title = get<1>(db_->movies_.GetKey(i));
        TemplateNode tmp;
        char bufid[30];
        snprintf(bufid, 30, "%llu", i);
        tmp.Insert("id", bufid);
        tmp.Insert("title", *title);
        lists.Insert(new TemplateNode(tmp));
    }
    nodes.Insert("result", new TemplateNode(lists));

    ret.clear();
    vector<string> failed_tags;
    if (generate_html(template_html, nodes, ret, failed_tags) < 0) {
        LOG_ERROR("Html template applied failed:");
        for(const auto &i : failed_tags)
            LOG_ERROR("bad tag(s): %s", i.c_str());
        ret.clear();
        return -1;
    }
    ret_type = "text/html";
    return 0;
}

int HttpHandler::proc_main(const unordered_map<string, string>& params, string& ret, string& ret_type)
{
    (void)params;
    string template_html;
    {
        auto it = html_templates_.find("main");
        if (it == html_templates_.end())
            return -1;
        template_html = it->second;
    }

    TemplateNode nodes;
    nodes.Insert("logo_and_searchbox", html_templates_["logo_and_searchbox"]);

    ret.clear();
    vector<string> failed_tags;
    if (generate_html(template_html, nodes, ret, failed_tags) < 0) {
        LOG_ERROR("Html template applied failed:");
        for(const auto &i : failed_tags)
            LOG_ERROR("bad tag(s): %s", i.c_str());
        ret.clear();
        return -1;
    }
    ret_type = "text/html";
    return 0;
}

int HttpHandler::proc_ico(const unordered_map<string, string>& params, string& ret, string& ret_type)
{
    (void)params;
    auto it = html_templates_.find("favicon");
    if (it == html_templates_.end())
        return -1;
    ret = it->second;
    ret_type = "image/jpeg";
    return 0;
}

}

