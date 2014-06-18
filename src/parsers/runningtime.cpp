#include "Parser.hpp"
#include <MovieDB.hpp>
#include <Log.hpp>

using namespace imdb;

void MoviesParser::Init() {
    	title_ = titleyear_ 
               = subtitle_ 
               = movietype_
               = time_ 
               = detail_ = "";
}

void MoviesParser::parseLine(const std::string input_line) {
	int input_len = input_line.length();
    //title counts the number of "
    //subtitle counts the number of {or}
    //year counts for the number of ( or ), which is outside the { }
    int title = 0, subtitle = 0, year = 0, movietype = 0;
    //ismovie tells this line is the desciption of a movie
    bool ismovie = true;
    //tell title has content or not
    bool hascontent = false;
    //tell title year has content or not
    bool titleyearhascontent = false;
    //istab tells whether meets the tab
    bool istab = false;
    //idxstart and idxend are used for marking the start and end of substring
    int idxstart = 0,len = 0;
    //tempstr stores the time part and detail part of the string, like (USA:60	(with commercial))
	string tempstr;
	
	//clear state
	Init();
	
    for (int i = 0; i < input_len; ++i) {
        ++len;
        switch(input_line[i]) {
            case '\"':
            {
            	//it is TV-serials
            	ismovie = false;
                if (title == 0) {
                    ++title;
                    idxstart = i + 1;
                    len = 0;
                    break;
                }
                //the substring between idxstart and idxend is title
                //process value and reset idxstart and idxend
                if(title == 1) {
                    title_ = input_line.substr(idxstart,len - 1);
                    len = 0;
                    title = 0;
                }
            }
            break;
            case '(':
            {
            	//means it is in title or subtitle
                if(title||subtitle) {
                    break;
                }
                else {
                	//if(ismovie && element.title.length() == 0) {
                	if(ismovie && !hascontent) {
                    	title_ = input_line.substr(idxstart,len - 1);
                    	hascontent = true;

                    }
                    if(year == 0 && !titleyearhascontent) {
                        ++year;
                        idxstart = i + 1;
                        len = 0;
                    }
                    if(ismovie && titleyearhascontent && movietype == 0) {
                    	++movietype;
                        idxstart = i + 1;
                        len = 0;
                    }          
                }
            }
            break;
            case ')':
            {
                //means it is in title or subtitle
                if (title||subtitle) {
                    break;
                }
                else {
                    if (year && movietype == 0 && !titleyearhascontent) {
                        titleyear_ = input_line.substr(idxstart,len - 1);
                        len = 0;
                        year = 0;
                        titleyearhascontent = true;
                    }
                    //output movie type
                    if (movietype && titleyearhascontent) {
                    	movietype_ = input_line.substr(idxstart,len - 1);
                        len = 0;
                        movietype = 0;
                    }
                }
            }
            break;
            case '{':
            {
                if (subtitle == 0) {
                    ++subtitle;
                    idxstart = i + 1;
                    len = 0;
                }
            }
            break;
            case '}':
            {
                //the substring between idxstart and idxend is title
                //process value and reset idxstart and idxend
                if(subtitle == 1) {
                    subtitle_ = input_line.substr(idxstart,len - 1);
                    len = 0;
                    subtitle = 0;
                }
            }
            break;
            case '\t':
            {
                istab = true;
                idxstart = i + 1;
            }
            break;
            default:
            {
                if (istab) {
                    tempstr = input_line.substr(idxstart,input_len - idxstart);                    
                    len = 0;
                    istab = false;
                    //tempstart only uses for tempstr
                    int tempstart = 0;
                    int tempstr_len = tempstr.length();
                    for (int i = 0; i < tempstr_len; ++i) {
                    	++len;
                    	if (tempstr[i] == '\t') {
                    		time_ = tempstr.substr(tempstart,len - 1);
                    		detail_ = tempstr.substr(len, tempstr_len - len);
                    		//to jump out of the for loop
                    		i = tempstr_len;
                    	}
                    	
                    }
                    //to jump out of the loop
                    i = input_len;
                }
                
            }
        } //end switch..case..
    }   // end for
 
    string key = title_ + "(" + titleyear_ + ")";
    auto db_ret = db_->movies_.GetInfo(key);
    if (get<0>(db_ret)) {
        // insert subtitle
        if (!time_.empty())
            get<2>(db_ret)->time_.push_back(time_);
    } else {
        //no such movie
    }
}

