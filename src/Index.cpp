#include <Index.hpp>
#include <unordered_set>
#include <algorithm>


namespace imdb{


void ReverseIndex::ShrinkMemory() 
{
    for (auto& it : rev_idx_)
        it.second.shrink_to_fit();

}

void ReverseIndex::Insert(const vector<string>& keys, size_t id) 
{
    idx_[id] = keys.size();
    for (const auto& s : keys)
        //be one of the candidates for every word
        if (!s.empty())
            rev_idx_[s].push_back(id);
}

void ReverseIndex::Clear() {
    rev_idx_.clear();
    idx_.clear();
}

vector<size_t> ReverseIndex::Lookup(const vector<string>& keys) {
    vector<size_t> ret;
    // mapping candidate to the number of query words it contained 
    unordered_map<size_t, size_t> cands;
    for (const auto& keyi : keys) {
        // reverse index has the query word
        if (rev_idx_.count(keyi)) {
            // insert into candidate id map, or increase the counter

            //dedup,  in case of 'Tiger Tiger Tiger'
            unordered_set<size_t> dedup;
            for (const auto& i : rev_idx_[keyi])
                dedup.insert(i);
            for (const auto& i : dedup)
                cands[i]++;
        }
    }

    // no result, return early
    if (cands.empty())
        return ret;

    // sort result by the number of query words they contained
    vector<pair<size_t, size_t>> rank;
    // move to vector for sort
    for (const auto& it : cands)
        rank.emplace_back(it.first, it.second);
    sort(rank.begin(), rank.end(),
            [&](const pair<size_t, size_t>& a, const pair<size_t, size_t>& b) {
                if (a.second > b.second)
                    return true;
                else if (a.second == b.second) {
                    if (idx_[a.first] < idx_[b.first])
                    return true;
                }
                return false;
            });

    // return result, ranked
    for (const auto& it : rank) {
        // if the first(best) result contained all query words, only return 
        // these best results,
        // Or return every candidates

        // search for "tiger+tiger" should not return "Tiger!" cuz it is too short, though it has high rank
        if ((it.second == keys.size() && idx_[it.first] >= keys.size()) || rank[0].second < keys.size())
            ret.push_back(it.first);
    }
    return ret;
}



} //namespace


