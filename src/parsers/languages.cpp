#include "Parser.hpp"
#include <MovieDB.hpp>

using namespace imdb;

void MoviesParser::Init() {
    	title_ = "";
    	titleyear_ = "";
    	subtitle_ = "";
    	movietype_ = "";
    	lang_ = ""
}

void MoviesParser::parseLine(const std::string input_line) {
	int strlen = input_line.length();
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
    //islang tells whether to start to record language
    bool islang = false;
    //idxstart and idxend are used for marking the start and end of substring
    int idxstart = 0,len = 0;
    for (int i = 0; i < strlen; ++i) {
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
                    //ofs<<" "<<input_line.substr(idxstart,len - 1);
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
                    	//ofs<<" "<<input_line.input_line(idxstart,len - 1);
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
                        //ofs<<" "<<input_line.substr(idxstart,len - 1);
                        len = 0;
                        year = 0;
                        titleyearhascontent = true;
                    }
                    //output movie type
                    if (movietype && titleyearhascontent) {
                    	movietype_ = input_line.substr(idxstart,len - 1);
                        //ofs<<" "<<input_line.substr(idxstart,len - 1);
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
                    //ofs<<" "<<input_line.substr(idxstart,len - 1);
                    len = 0;
                    subtitle = 0;
                }
            }
            break;
            case '\t':
            {
                islang = true;
                idxstart = i + 1;
            }
            break;
            default:
            {
                if (islang) {
                    lang_ = input_line.substr(idxstart,strlen - idxstart);
                    //ofs<<" "<<input_line.substr(idxstart,strlen - idxstart)<<"\n";
                    len = 0;
                    islang = false;
                    //to jump out of the loop
                    i = strlen;
                }
                
            }
        }
    }    
}
