#include <DataSchema.hpp>
#include <Index.hpp> //for NULLID
#include <algorithm>


using namespace std;

namespace imdb{

void Movie::AddActor(size_t id, int pos)
{
    actors_rank_.push_back(make_pair(id, pos));
}

void Movie::AddDirector(size_t id)
{
    directors_.push_back(id);
}

void Movie::RankPeople()
{
    sort(actors_rank_.begin(), actors_rank_.end(), 
            [](const pair<size_t, int>& a, const pair<size_t, int>& b) {
                return a.second < b.second;
            });
    for(const auto& i: actors_rank_)
        actors_.push_back(i.first);
}

}
