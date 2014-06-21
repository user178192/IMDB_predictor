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



}
