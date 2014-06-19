#ifndef _INDEX_HPP_
#define _INDEX_HPP_
#include <unordered_map>
#include <tuple>
#include <vector>
#include <string>
#include <algorithm>

namespace imdb {

    using namespace std;
    static size_t NULLID = -1;

    template<bool ReadOnly>
    class Serializer;

    class ReverseIndex {
        template<bool ReadOnly>
        friend class Serializer;
    private:
        // reverse index, mapping keywords to all the 
        // candidates id that contains the corresponding keyword 
        unordered_map<string, vector<size_t>> rev_idx_;
        // index, mapping id to the keywords under it
        unordered_map<size_t, vector<string>> idx_;

    public:

        void ShrinkMemory() {
            for (auto& it : rev_idx_)
                it.second.shrink_to_fit();
            for (auto& it : idx_)
                it.second.shrink_to_fit();
        }

        void Insert(const vector<string>& keys, size_t id) {
            idx_[id] = keys;
            for (const auto& s : keys)
                //be one of the candidates for every word
                rev_idx_[s].push_back(id);
        }

        void Clear() {
            rev_idx_.clear();
            idx_.clear();
        }

        vector<size_t> Lookup(const vector<string>& keys) {
            vector<size_t> ret;
            // mapping candidate to the number of query words it contained 
            unordered_map<size_t, size_t> cands;
            for (const auto& keyi : keys) {
                // reverse index has the query word
                if (rev_idx_.count(keyi))
                    // insert into candidate id map, or increase the counter
                    for (const auto& i : rev_idx_[keyi])
                        cands[i]++;
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
                        return a.second > b.second;
                    });

            // return result, ranked
            for (const auto& it : rank) {
                // if the first(best) result contained all query words, only return 
                // these best results,
                // Or return every candidates
                if (it.second == keys.size() || rank[0].second < keys.size())
                    ret.push_back(it.first);
            }
            return ret;
        }
    };

    template <typename TKey, typename TVal>
    class Index {
        template<bool ReadOnly>
        friend class Serializer;
    private:
        // map of Key to internal ID , internal id is auto generated and auto increment,
        // it increases when inserting inexisting key 
        unordered_map<TKey, size_t> k2id_;

        // store the keys and values, index by internel id 
        vector<pair<TKey, TVal>> id2val_;

    public:

        void ShrinkMemory() {
            id2val_.shrink_to_fit();
        }

        size_t Size() const {
            return id2val_.size();
        }

        // retrieve record by internel ID, return triplet of:
        // (have result?,  key, pointer of value)
        // value can be modified by the pointer

        tuple<bool, const TKey*, TVal*> GetInfo(const size_t id) {
            tuple<bool, const TKey*, TVal*> ret;
            if (id >= id2val_.size()) {
                // id out of range 
                get<0>(ret) = false;
                get<1>(ret) = nullptr;
                get<2>(ret) = nullptr;
            } else {
                get<0>(ret) = true;
                get<1>(ret) = &(id2val_[id].first);
                get<2>(ret) = &(id2val_[id].second);
            }
            return move(ret);
        }

        // retrieve record by Key,  similiar to GetInfo(const size_t id)
        // return triplet.
        // if not exist, first element of triplet is false

        tuple<bool, size_t, TVal*> GetInfo(const TKey &key) {
            auto it = k2id_.find(key);
            tuple<bool, size_t, TVal*> ret;

            if (it == k2id_.end()) {
                // inexists
                get<0>(ret) = false;
                get<1>(ret) = NULLID;
                get<2>(ret) = nullptr;
            } else {
                get<0>(ret) = true;
                get<1>(ret) = it->second;
                get<2>(ret) = &(id2val_[it->second].second);
            }

            return move(ret);
        }

        // get internel id by key

        tuple<bool, size_t> GetID(const TKey &key) {
            tuple<bool, size_t> ret;
            auto it = k2id_.find(key);
            if (it != k2id_.end()) {
                get<0>(ret) = true;
                get<1>(ret) = it->second;
            } else {
                // inexists
                get<0>(ret) = false;
                get<1>(ret) = NULLID;
            }
            return move(ret);
        }

        // get key by internal id 

        tuple<bool, const TKey*> GetKey(const size_t id) {
            tuple<bool, const TKey*> ret;
            if (id >= id2val_.size()) {
                get<0>(ret) = false;
                get<1>(ret) = nullptr;
            } else {
                get<0>(ret) = true;
                get<1>(ret) = &(id2val_[id].first);
            }
            return move(ret);
        }

        // insert new record, key and value, return the internal id of the inserted one
        // will overwrite previous one if the key already exists

        size_t Insert(const TKey &key, const TVal &val) {
            auto it = k2id_.find(key);
            size_t ret = id2val_.size();
            if (it != k2id_.end()) {
                id2val_[it->second].second = val;
                ret = it->second;
            } else {
                k2id_.insert(make_pair(key, id2val_.size()));
                id2val_.emplace_back(key, val);
            }
            return ret;
        }
    };

} //namespace


#endif

