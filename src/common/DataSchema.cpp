#include <common/DataSchema.hpp>
#include <common/Index.hpp> //for NULLID
#include <algorithm>
#include <unordered_set>


using namespace std;

namespace imdb {

    void Movie::AddActor(size_t id, int pos) {
        actors_rank_.push_back(make_pair(id, pos));
    }

    void Movie::AddDirector(size_t id) {
        directors_.push_back(id);
    }

    void Movie::RankPeople() {
        sort(actors_rank_.begin(), actors_rank_.end(),
                [](const pair<size_t, int>& a, const pair<size_t, int>& b) {
                    return a.second < b.second;
                });

        unordered_set<size_t> dedup;
        for (const auto& i : actors_rank_) {
            // no duplicate actors inside
            if (dedup.count(i.first))
                continue;
            actors_.push_back(i.first);
            dedup.insert(i.first);
        }
    }

}
