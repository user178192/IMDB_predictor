#include "Parser.hpp"
#include <MovieDB.hpp>
#include <Log.hpp>

using namespace imdb;

void MoviesParser::Init() {
    	title_ = subtitle_ 
               = type_ 
               = year_ = "";
}

void MoviesParser::Finish() 
{
    LOG_INFO("Read in %llu movies", db_->movies_.Size());
}

void Moviesparser::ProcessPart1(string part1)
{
	int idxstart = 0,idxend = part1.length() - 1;
	bool findlb = false,findrb = false;
    int stkbrace = 0;
    int stkpare = 0;
	int endbrace = 0;
	int endpare = 0;
	bool istype = false;
	size_t found;	
	
	for (int i = idxend; i >= 0; --i) {
		if (part1[i] == '}') {
			if (stkbrace == 0)
				endbrace = i;
			++stkbrace;
		}
		if(part1[i] == ')') {
			if (stkpare == 0)
				endpare = i;
			++stkpare;
			if (!isdigit(part1[i-1])) {
				//is type
				istype = true;	
			} else {
				istype = false;
				title_ = part1.substr(1,i);
				found = title_.find("\" (");
				if(found != string::npos) {
					title_.replace(found,3," (");
				}
			}
		}
		if (part1[i] == '{') {
			--stkbrace;
			if (stkbrace == 0)
				subtitle_ = part1.substr(i+1,endbrace-i-1);
		}
		if (part1[i] == '(') {
			--stkpare;
			if (stkpare == 0) {
				if (istype) {
					type_ = part1.substr(i+1,endpare-i-1);
					title_ = part1.substr(1,i-1);
					found = title_.find("\" (");
					if(found != string::npos) {
						title_.replace(found,3," (");
					}
				}
			}
		}
	}
}

void MoviesParser::parseLine(const std::string input_line) {
	int input_len = input_line.length();
    
	string part1;
    // clear the state
    Init();
       
	for (int i = 0; i < input_len; ++i) {
        if (vstr[i] == '\t') {
            part1 = input_line.substr(0,i);
            while(vstr[i] == '\t')
            	++i;
            year_ = input_line.substr(i,strlen - i);
            //ofs<<" "<<vstr.substr(i,strlen - i)<<"\n";
            i = strlen;
        }                            
    }//end of for
        
    ProcessPart1(part1);

    //m.year_ = 
    string key = title_;
    auto db_ret = db_->movies_.GetInfo(key);
    if (get<0>(db_ret)) {
        // already exists, and have subtitle, (series)
        // insert subtitle
        if (!subtitle_.empty())
            get<2>(db_ret)->subtitles_.push_back(subtitle_);
    } else {
        // insert new entry
        Movie m;
        m.year_ = year_;
        m.type_ = movietype_;
        db_->movies_.Insert(key, m);
    }
}

