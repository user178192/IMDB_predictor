#include <relation/MovieRelation.hpp>
#include <common/MovieDB.hpp>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <algorithm>
#include <cmath>
#include <cstdio>


namespace imdb
{
    
    
    size_t MovieRelation::SimilarityScore(const Movie& ma, const Movie& mb)
    {
        size_t score = 0;
        unordered_map<size_t, int> ha, hb;
        unordered_map<string, int> hsa, hsb;
        if (ma.composer_ == mb.composer_)
            score += composer_power;  
        
        // co-occurance of actors, score ranked by order
        for(size_t i = 0; i < ma.actors_.size(); i++) {
            ha[ma.actors_[i]] = i < 5 ? 5 - i : 1;
        }
        
        for(size_t i = 0; i < mb.actors_.size(); i++) {
            hb[mb.actors_[i]] = i < 5 ? 5 - i : 1;
        }
        
        for(const auto& i : ha) {
            if (hb.count(i.first)) {
                score += i.second * hb[i.first];
            }
        }
        
        ha.clear(); hb.clear();
        
        // co-occurance of directors
        for(size_t i = 0; i < ma.directors_.size(); i++) {
            ha[ma.directors_[i]] = i < 5 ? 5 - i : 1;
        }
        
        for(size_t i = 0; i < mb.directors_.size(); i++) {
            hb[mb.directors_[i]] = i < 5 ? 5 - i : 1;
        }
        
        for(const auto& i : ha) {
            if (hb.count(i.first)) {
                score += i.second * hb[i.first];
            }
        }
        
        ha.clear(); hb.clear();
        
        // co-occurance of directors
        for(size_t i = 0; i < ma.genres_.size(); i++) {
            hsa[ma.genres_[i]] = 4;
        }
        
        for(size_t i = 0; i < mb.genres_.size(); i++) {
            hsb[mb.genres_[i]] = 4;
        }
        
        for(const auto& i : hsa) {
            if (hsb.count(i.first)) {
                score += i.second * hsb[i.first];
            }
        }
        
        hsa.clear(); hsb.clear();
        
        //how much votes differs matters
        if (ma.votes_ > 5000 && mb.votes_ < 5000) {
            score /= (1 + abs(log(ma.votes_) / log(10) - log(mb.votes_) / log(10)));
        }
        
        return score;
    }
    
    
    
    std::vector<size_t> MovieRelation::GetSimilarMovies(const Movie& m, MovieDB *mdb)
    {
        unordered_set<size_t> cands;
        // get all candidates movie, by getting other works of
        // current movie's actors/directors'
        for(const auto i : m.actors_) {
            People *p = get<2>(mdb->actors_.GetInfo(i));
            if (p == nullptr) { continue; }
            for(const auto j : p->movies_) {
                cands.insert(j);
            }
        }
        
        for(const auto i : m.directors_) {
            People *p = get<2>(mdb->directors_.GetInfo(i));
            if (p == nullptr) { continue; }
            for(const auto j : p->movies_) {
                cands.insert(j);
            }
        }
        
        vector<pair<size_t, size_t>> results;
        // compute similarities of every candidates
        for(const auto i : cands) {
            results.push_back(make_pair(i, SimilarityScore(m, *get<2>(mdb->movies_.GetInfo(i)))));
        }
        
        //sort result
        sort(results.begin(), results.end(), [&](const pair<size_t, size_t>& pa,
                                                 const pair<size_t, size_t>& pb) {
            return pa.second > pb.second;
        });
        
        vector<size_t> ret;
        //eliminate itself at position 0
        for(size_t i = 1; i < simil_size + 1 && i < results.size(); i++) {
            ret.push_back(results[i].first);
        }
        
        return move(ret);
    }

    void MovieRelation::GetAllSimilarity(MovieDB *mdb)
    {
        printf("Begin compute similarity for all movies..\n");
        auto &movies = mdb->movies_;
        for(size_t i = 0; i < movies.Size(); i++) {
            if (i % 10 == 1)
                printf("\rProgress %llu / %llu\t\t\t", i, movies.Size());

            // computation is expensive, ignore some movies
            if (movies[i].votes_ < 1000)
                continue;

            movies[i].relation_ = GetSimilarMovies(movies[i], mdb);
        }
        printf("\rProgress %llu / %llu\t\t\t\n", movies.Size(), movies.Size());
    }

    void MovieRelation::GetAllRatingPred(MovieDB *mdb)
    {
        (void)mdb;
        return;
    }
    
}


