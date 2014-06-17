#include <DataSchema.hpp>

namespace imdb{

void Movie::AddActor(size_t id, int pos)
{
    while(actors_.size() < pos)
        actors_.push_back(NULLID);
    actors_[pos - 1] = id;
}

void Movie::AddDirector(size_t id, int pos)
{
    while(directors_.size() < pos)
        directors_.push_back(NULLID);
    directors_[pos - 1] = id;
}

};
