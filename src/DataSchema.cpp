#include <DataSchema.hpp>
#include <Index.hpp> //for NULLID

namespace imdb{

void Movie::AddActor(size_t id, size_t pos)
{
    while(actors_.size() < pos)
        actors_.push_back(NULLID);
    actors_[pos - 1] = id;
}

void Movie::AddDirector(size_t id, size_t pos)
{
    while(directors_.size() < pos)
        directors_.push_back(NULLID);
    directors_[pos - 1] = id;
}

}
