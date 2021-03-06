#include <common/MovieDB.hpp>
#include <common/Serializer.hpp>
#include <common/Log.hpp>
#include <algorithm>
#include <cctype>

namespace imdb {

void MovieDB::BuildIndex() {
    LOG_INFO("Begin");
    ri_movies_.Clear();
    for (size_t i = 0; i < movies_.Size(); i++) {
        ri_movies_.Insert(split_string(*(get<1>(movies_.GetKey(i))), " ,!#'(){}/"), i);
    }
    ri_movies_.ShrinkMemory();

    /*
    ri_actors_.Clear();
    for (size_t i = 0; i < actors_.Size(); i++) {
        ri_actors_.Insert(split_string(*(get<1>(actors_.GetKey(i))), " ,!#'(){}/"), i);
    }
    ri_actors_.ShrinkMemory();

    ri_directors_.Clear();
    for (size_t i = 0; i < directors_.Size(); i++) {
        ri_directors_.Insert(split_string(*(get<1>(directors_.GetKey(i))), " ,!#'(){}/"), i);
    }
    ri_directors_.ShrinkMemory();

    ri_composers_.Clear();
    for (size_t i = 0; i < composers_.Size(); i++) {
        ri_composers_.Insert(split_string(*(get<1>(composers_.GetKey(i))), " ,!#'(){}/"), i);
    }
    ri_composers_.ShrinkMemory();
    */

    LOG_INFO("Done");
}

int MovieDB::LoadFromFile(const std::string& filename) {
    auto *reader = new Serializer<true>(filename);
    if (!reader->IsValid()) {
        LOG_ERROR("Load Movie DB File failed");
        delete reader;
        return -1;
    }

    /*
    reader->Read(ri_movie_);
    reader->Read(ri_time_);
    reader->Read(ri_people_);
    */

    reader->Read(movies_);
    reader->Read(actors_);
    reader->Read(composers_);
    reader->Read(directors_);
    LOG_INFO("Load DB done, %llu movies, %llu people",
            movies_.Size(), actors_.Size() + composers_.Size() + directors_.Size());
    delete reader;
    movies_.ShrinkMemory();
    actors_.ShrinkMemory();
    composers_.ShrinkMemory();
    directors_.ShrinkMemory();
    return 0;
}

int MovieDB::SaveToFile(const std::string& filename) {
    auto *writer = new Serializer<false>(filename);
    if (!writer->IsValid()) {
        LOG_ERROR("Open Movie DB File for write failed");
        delete writer;
        return -1;
    }

    for (size_t i = 0; i < movies_.Size(); i++) {
        auto m = get<2>(movies_.GetInfo(i));
        m->RankPeople();
        sort(m->languages_.begin(), m->languages_.end());
        m->languages_.erase(unique(m->languages_.begin(), m->languages_.end()), m->languages_.end());

        sort(m->countries_.begin(), m->countries_.end());
        m->countries_.erase(unique(m->countries_.begin(), m->countries_.end()), m->countries_.end());

        sort(m->directors_.begin(), m->directors_.end());
        m->directors_.erase(unique(m->directors_.begin(), m->directors_.end()), m->directors_.end());

        sort(m->genres_.begin(), m->genres_.end());
        m->genres_.erase(unique(m->genres_.begin(), m->genres_.end()), m->genres_.end());
    }

    /*
    writer->Write(ri_movie_);
    writer->Write(ri_time_);
    writer->Write(ri_people_);
    */

    writer->Write(movies_);

    for (size_t i = 0; i < actors_.Size(); i++) {
        auto p = get<2>(actors_.GetInfo(i));
        sort(p->movies_.begin(), p->movies_.end());
        p->movies_.erase(unique(p->movies_.begin(), p->movies_.end()), p->movies_.end());
    }
    writer->Write(actors_);
    for (size_t i = 0; i < composers_.Size(); i++) {
        auto p = get<2>(composers_.GetInfo(i));
        sort(p->movies_.begin(), p->movies_.end());
        p->movies_.erase(unique(p->movies_.begin(), p->movies_.end()), p->movies_.end());
    }
    writer->Write(composers_);
    for (size_t i = 0; i < directors_.Size(); i++) {
        auto p = get<2>(directors_.GetInfo(i));
        sort(p->movies_.begin(), p->movies_.end());
        p->movies_.erase(unique(p->movies_.begin(), p->movies_.end()), p->movies_.end());
    }
    writer->Write(directors_);
    LOG_INFO("Save DB done, %llu movies, %llu people",
            movies_.Size(), actors_.Size() + composers_.Size() + directors_.Size());
    delete writer;
    return 0;
}

string MovieDB::NameReorder(const string& name)
{
    string last, first;
    const string &s(name);
    bool infirst = true;
    for(size_t i = 0; i < name.size(); i++) {
        if (s[i] == ',' && s[i + 1] == ' ') {
            infirst = false;
            i++;
            continue;
        }

        // (I) (II)
        if (s[i] == '(')
            break;

        if (infirst)
            first.append(1, s[i]);
        else
            last.append(1, s[i]);
    }
    last.append(1, ' ');
    return move(last + first);
}

}


#if 0

using namespace imdb;

#include <iostream>

int main() {
    ReverseIndex ri;
    ri.Insert(split_string("Edge of Tomorrow", " \t.,"), 1);
    ri.Insert(split_string("Edge AND Tomorrow", " \t.,"), 2);
    ri.Insert(split_string("Edge of YesterDAY", " \t.,"), 3);
    ri.Insert(split_string("Edge", " \t.,"), 4);

    auto ret = ri.Lookup({"edge", "of", "tomorrow"});
    cout << "query 'edge of tomorrow': ";
    for (const auto i : ret)
        cout << i << '\t';
    cout << endl;

    ret = ri.Lookup({"edge", "of", "yesterday"});
    cout << "query 'edge of yesterday': ";
    for (const auto i : ret)
        cout << i << '\t';
    cout << endl;

    ret = ri.Lookup({"edge"});
    cout << "query 'edge': ";
    for (const auto i : ret)
        cout << i << '\t';
    cout << endl;

    ret = ri.Lookup({"and"});
    cout << "query 'and': ";
    for (const auto i : ret)
        cout << i << '\t';
    cout << endl;

    return 0;

}

#endif

