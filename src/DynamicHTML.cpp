#include <DynamicHTML.hpp>


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
    val_map_.insert(make_pair(key, t));
}

void TemplateNode::Insert(const std::string& key, const std::string& val)
{
    assert(type_ == kNone || type_ == kMap);
    type_ = kMap;
    auto *t = new TemplateNode(val);
    val_map_.insert(make_pair(key, t));
}

int generate_html(const std::string& template_html, 
    const TemplateNode& nodes,
    std::string& output, std::vector<std::string>& failed_tags)
{

    return 0;
}

}


using namespace dynamichtml;
using namespace std;
int main()
{
    TemplateNode n;
    n.Insert("sb", "haha");
    n.Insert("sb2", "haha2");
    n.Insert("sb3", "haha3");
    n.Insert("sb4", new TemplateNode(n));
    

    string ret;
    vector<string> failed_tags;
    auto m = n;
    generate_html("asdf", m, ret, failed_tags);
}

