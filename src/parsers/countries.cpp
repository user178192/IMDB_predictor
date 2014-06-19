#include "Parser.hpp"
#include <MovieDB.hpp>
#include <Log.hpp>

using namespace imdb;

void CountriesParser::Init() {
    	title_ = titleyear_ 
    			= subtitle_
    	 		= type_
    			= country_ = "";
}

void CountriesParser::ProcessPart1(string part1)
{
	int idxend = part1.length() - 1;
    int stkbrace = 0;
    int stkpare = 0;
	int endbrace = 0;
	int endpare = 0;
	bool istype = false;
	size_t found;	
    bool inbrace = false;
	
	for (int i = idxend; i >= 0; --i) {
		if (part1[i] == '}') {
            inbrace = true;
			if (stkbrace == 0)
				endbrace = i;
			++stkbrace;
		}
		if(part1[i] == ')') {
            if(!inbrace){
                
			if (stkpare == 0)
				endpare = i;
			++stkpare;
			if (!isdigit(part1[i-1])) {
				//is type
				istype = true;	
			} else {
				istype = false;
				title_ = part1.substr(0,i);
				found = title_.find("\" (");
				if(found != string::npos) {
					title_.replace(found,3," (");
				}
                i = -1;
			}
            }
		}
		if (part1[i] == '{') {
            inbrace = false;
			--stkbrace;
			if (stkbrace == 0)
				subtitle_ = part1.substr(i+1,endbrace-i-1);
		}
		if (part1[i] == '(') {
            if(!inbrace){
			--stkpare;
			if (stkpare == 0) {
				if (istype) {
                    //it is movie
					type_ = part1.substr(i+1,endpare-i-1);
					title_ = part1.substr(0,i-1);
					found = title_.find("\" (");
					if(found != string::npos) {
						title_.replace(found,3," (");
					}
                    i = -1;
				}
			}
            }   
		}
	}
	if(title_[0] == '\"')
		title_ = title_.substr(1,title_.length() - 1);
}

void CountriesParser::parseLine(const std::string input_line) {
	int input_len = input_line.length();
    string part1;
    // clear the state
    Init();
    
    for (int i = 0; i < input_len; ++i) {
        if (input_line[i] == '\t') {
            part1 = input_line.substr(0,i);
            while(input_line[i] == '\t')
            	++i;
            country_ = input_line.substr(i,input_len - i);
            //ofs<<" "<<vstr.substr(i,strlen - i)<<"\n";
            i = input_len;
        }                            
    }//end of for
        
    ProcessPart1(part1);
    
    //cout<<title_<<" "<<subtitle_<<" "<<type_<<" ["<<country_<<"]\n";
    
    string key = title_;
    auto db_ret = db_->movies_.GetInfo(key);
    if (get<0>(db_ret)) {
        // insert languages, since one movie may have mutiple languages
        if (!country_.empty())
        	get<2>(db_ret)->countries_.push_back(country_);
    } else {
        // no such movie, inconsistant, ignore
        LOG_DEBUG("Movie [%s] not found, inconsistant", key.c_str());
    }
}
