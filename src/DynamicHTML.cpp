#include <DynamicHTML.hpp>
#include <cstring>


using namespace std;

namespace dynamichtml{


TemplateNode::TemplateNode()
    : type_(kNone)
{
}

TemplateNode::TemplateNode(const std::string& val)
    : type_(kString), val_string_(val)
{
}

TemplateNode::~TemplateNode()
{
    if (type_ == kVector) {
        for(auto i : val_vector_)
            delete i;
    } else if (type_ == kMap) {
        for(auto i : val_map_)
            delete i.second;
    }
}

TemplateNode::TemplateNode(const TemplateNode& t)
    : type_(t.type_), val_string_(t.val_string_),
    val_map_(t.val_map_), val_vector_(t.val_vector_)
{
    for(auto &i : val_vector_)
        i = new TemplateNode(*i);
    
    for(auto &i : val_map_)
        i.second = new TemplateNode(*(i.second));
}

TemplateNode& TemplateNode::operator=(const TemplateNode& t)
{
    type_ = t.type_;
    val_string_ = t.val_string_;
    val_map_ = t.val_map_;
    val_vector_ = t.val_vector_;
    for(auto &i : val_vector_)
        i = new TemplateNode(*i);
    
    for(auto &i : val_map_)
        i.second = new TemplateNode(*(i.second));
    return *this;
}

void TemplateNode::Insert(TemplateNode *t)
{
    assert(type_ == kNone || type_ == kVector);
    type_ = kVector;
    val_vector_.push_back(t);
}

void TemplateNode::Insert(const std::string& key, TemplateNode *t) 
{
    assert(type_ == kNone || type_ == kMap);
    assert(t != this);
    type_ = kMap;
    auto it = val_map_.find(key);
    if (it != val_map_.end()) {
        if (t != it->second)
            delete it->second;
        it->second = t;
    } else {
        val_map_.insert(make_pair(key, t));
    }
}

void TemplateNode::Insert(const std::string& key, const std::string& val)
{
    assert(type_ == kNone || type_ == kMap);
    type_ = kMap;
    auto *t = new TemplateNode(val);
    Insert(key, t);
}

void TemplateNode::Clear()
{
    if (type_ == kVector) {
        for(auto i : val_vector_)
            delete i;
    } else if (type_ == kMap) {
        for(auto i : val_map_)
            delete i.second;
    }
    type_ = kNone;
    val_string_.clear();
    val_vector_.clear();
    val_map_.clear();
}

int generate_html(const std::string& s, 
    const TemplateNode& nodes,
    std::string& output, std::vector<std::string>& failed_tags)
{
    int ret = 0;
    if (nodes.type_ != TemplateNode::kMap
            && nodes.type_ != TemplateNode::kNone)
        ret = -1;

    for(size_t i = 0; i  < s.size();) {

        //tag found
        if (i + 4 < s.size() && s[i] == '[' && s[i + 1] == '&') {

            //find end of current tag, care about recursive tags.
            auto find_right = [&](const char *left, const char *needle) {
                size_t sleft = (left ? strlen(left) : 0), sneedle = strlen(needle);
                size_t j = i;
                int sstack = 1;
                while(j < s.size()) {
                // dont care about safety for simplicity
                    if (left && (strncmp(left, &s[j], sleft) == 0)) {
                        sstack++;
                    } else if (strncmp(needle, &s[j], sneedle) == 0) {
                        sstack--;
                        if (sstack == 0) {
                            break;
                        }
                    }
                    j++;
                }
                return j;
            };
            
            //special tag
            i += 2;
            if (s[i] == '&') {
                // is vector
                i++;
                // find key    ' [&&key&  '
                size_t j = find_right(nullptr, "&");
                string key = s.substr(i, j - i);
                i = j + 1;
                j = find_right("[&", "&]");
                // range error
                if (i >= s.size()) {
                    failed_tags.push_back("Bad template");
                    ret = -1;
                    break;
                }
                // cut down the recursive part
                string subtemplate = s.substr(i, j - i);
                i = j + 2;

                auto it = nodes.val_map_.find(key);
                if (it == nodes.val_map_.end()) {
                    // key not found, can not replace
                    failed_tags.push_back(key);
                    ret = -1;
                    continue;
                } 

                for(auto i : it->second->val_vector_) {
                    string suboutput;
                    int ret2 = generate_html(subtemplate, *i, suboutput, failed_tags);
                    if (!ret2)
                        ret = ret2;
                    output.append(suboutput);
                }
            } else {
                size_t j = find_right("[&", "&]");
                string key = s.substr(i, j - i);
                i = j + 2;

                auto it = nodes.val_map_.find(key);
                if (it == nodes.val_map_.end()) {
                    // key not found, can not replace
                    failed_tags.push_back(key);
                    ret = -1;
                    continue;
                } 
                output.append(it->second->val_string_);
            }
        } else {
            output.append(1, s[i]);
            i++;
        }
    }

    return ret;
}

}


/*
#include <iostream>
using namespace dynamichtml;
int main()
{
    TemplateNode n;
    n.Insert("ka", "vala");
    n.Insert("kb", "valb");
    TemplateNode tmp, vec;
    tmp.Insert("m1", "2001");
    vec.Insert(new TemplateNode(tmp));
    tmp.Insert("m1", "2002");
    vec.Insert(new TemplateNode(tmp));
    tmp.Insert("m1", "2003");
    vec.Insert(new TemplateNode(tmp));
    tmp.Insert("m1", "2004");
    vec.Insert(new TemplateNode(tmp));
    n.Insert("kc", new TemplateNode(vec));

    string temp = "<html><body>\n"
        "<h1>[&ka&]<h1>\n"
        "<h2>[&kb&]<h2>\n"
        "[&&kc&<td>[&m1&]</td>\n&]"
        "</body></html>";
    string ret;

    vector<string> failed_tags;
    auto m = n;


    int succ = generate_html(temp, m, ret, failed_tags);
    cout << "succ: " << succ << endl;
    if (succ < 0) {
        cout << "ERROR:" << endl;
        for(const auto& i : failed_tags)
            cout << i << endl;
        cout << "===" << endl;
    }
    cout << ret << endl;
    return 0;
}

*/
