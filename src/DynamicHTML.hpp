#ifndef _DYNAMICHTML_HPP_
#define _DYNAMICHTML_HPP_
#include <cassert>
#include <vector>
#include <string>
#include <unordered_map>

namespace dynamichtml{

class TemplateNode
{
    // three conditions
    // string
    // unordered_map<string, template *>
    // vector<template *>

    enum {
        kNone,
        kString,
        kVector,
        kMap,
    } type_;


    /*
       find string xx; 
    */
    std::string val_string_;
    std::unordered_map<std::string, TemplateNode*> val_map_;
    std::vector<TemplateNode*> val_vector_;

    friend int generate_html(const std::string& template_html, 
            const TemplateNode& nodes,
            std::string& output, 
            std::vector<std::string>& failed_tags);

public:
    TemplateNode();
    TemplateNode(const std::string& val);
    TemplateNode(const TemplateNode&);
    ~TemplateNode();
    TemplateNode& operator=(const TemplateNode&);

    void Insert(TemplateNode *t);
    void Insert(const std::string& key, TemplateNode *t);
    void Insert(const std::string& key, const std::string& val);
};


int generate_html(const std::string& template_html, 
    const TemplateNode& nodes,
    std::string& output, 
    std::vector<std::string>& failed_tags)
{

}

}



#endif

