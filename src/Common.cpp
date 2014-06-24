#include <Common.hpp>


namespace imdb{

vector<string> split_string(const string& s, const string& delim, size_t max_split) {
    vector<string> ret;

    string cur;
    for (const char c : s) {
        bool isdelim = false;

        if (ret.size() >= max_split - 1) {
            cur.append(1, ::tolower(c));
            continue;
        }

        for (const char d : delim)
            if (d == c) {
                isdelim = true;
                break;
            }

        if (isdelim) {
            if (!cur.empty()) {
                ret.push_back(cur);
                cur.clear();
            }
        } else
            cur.append(1, ::tolower(c));
    }
    if (!cur.empty())
        ret.push_back(cur);

    return move(ret);
}

std::vector<std::string> split(const std::string &input_line, const char &delim ) {
    auto begin = input_line.begin();
    vector<string> tokens;

    while(begin < input_line.end()) {
        auto temp = find(begin, input_line.end(), delim);
        if(begin < input_line.end()) {
            tokens.push_back(string(begin, temp));
        }

        begin = temp;
        while ((begin < input_line.end()) && (*begin == delim)) {
            begin++;
        }
    }
    return move(tokens);
}

size_t find_year_pos(const std::string input_line, size_t begin) {
        size_t flag = begin;
        size_t left_pos = begin, right_pos = begin;
        for(size_t index = begin ; index < input_line.length(); index++) {
            if (input_line[index] == '(') {left_pos = index;}
            if (input_line[index] == ')') {right_pos = index;}

            if (right_pos > left_pos && right_pos - left_pos >= 5) {
                std::string temp(input_line, left_pos + 1, right_pos - left_pos - 1);
                // (1990/IV)
                if (temp.length() > 4) {
                    if (temp.find_first_not_of("0123456789?/IVX") == std::string::npos) {
                        flag = right_pos;
                        break;
                    }
                }
                else if (temp.length() == 4) {
                    if (temp.find_first_not_of("0123456789?") == std::string::npos) {
                        flag = right_pos;
                        break;
                    }
                }
            }
        }
        return flag;       
}


}
