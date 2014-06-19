#ifndef _SERIALIZER_HPP_
#define _SERIALIZER_HPP_

#include <DataSchema.hpp>
#include <Index.hpp>
#include <Log.hpp>

#include <vector>
#include <string>
#include <unordered_map>
#include <sys/mman.h>
#include <sys/stat.h>
#include <cerrno>
#include <cstdlib>
#include <cstring>
#include <fcntl.h>
#include <unistd.h>

namespace imdb {


    using namespace std;

    template<bool ReadOnly = false >
    class Serializer {
    private:
        const string filename_;
        int fd_;

        static const int kBS = 8192;
        char block_[kBS];
        char *s_;
        char *sbase_;
        size_t fsize_;

        /* make sure buffer has enough space to put in a new length of data */
        void check_flush(int i) {
            if (kBS - (s_ - block_) < i) {
                int size = s_ - block_;
                int ret = write(fd_, block_, size);
                if (ret != size) {
                    LOG_ERROR("Exception: write failed ret %d while writing %d",
                            ret, size);
                }
                fsize_ += size;
                s_ = block_;
            }
        };

    public:

        Serializer(const string& filename)
        : filename_(filename), fd_(-1), fsize_(0) {
            int oflag;
            if (ReadOnly)
                oflag = O_RDONLY;
            else
                oflag = O_WRONLY | O_CREAT | O_TRUNC;

            fd_ = open(filename_.c_str(), oflag, 0644);
            if (fd_ < 0) {
                LOG_ERROR("Save DB, %s open failed, [%s]",
                        filename_.c_str(), strerror(errno));
                return;
            }

            if (ReadOnly) {
                fsize_ = lseek(fd_, 0, SEEK_END);
                s_ = sbase_ = (char*) mmap(nullptr, fsize_, PROT_READ, MAP_PRIVATE, fd_, 0);
                if (!s_) {
                    close(fd_);
                    fd_ = -1;
                    return;
                }
            } else {
                s_ = block_;
            }
        }

        bool IsValid() {
            return fd_ >= 0;
        }

        ~Serializer() {
            if (fd_ >= 0) {
                if (ReadOnly)
                    munmap(sbase_, fsize_);
                else
                    check_flush(kBS + 1);
                close(fd_);
                LOG_DEBUG("file %s done, size: %llu", filename_.c_str(), fsize_);
            }
        }

        template <typename TVal>
        void Write(const TVal &val) {
            static_assert(ReadOnly == false, "Do not write in ReadOnly mode");
            check_flush(sizeof (val));
            *(TVal*) s_ = val;
            s_ += sizeof (val);
        }

        template <typename TVal>
        void Read(TVal &val) {
            static_assert(ReadOnly == true, "Do not read in write mode");
            val = *(TVal*) s_;
            s_ += sizeof (val);
        }

        void Write(const string &val) {
            Write(val.size());
            check_flush(val.size());
            if (val.size() >= kBS) {
                write(fd_, val.c_str(), val.size());
                fsize_ += val.size();
            } else {
                memcpy(s_, val.c_str(), val.size());
                s_ += val.size();
            }
        }

        void Read(string &val) {
            size_t len;
            Read(len);
            val.assign(s_, len);
            s_ += len;
        }

        template<typename TVal>
        void Write(const vector<TVal> &val) {
            Write(val.size());
            for (const auto& item : val)
                Write(item);
        }

        template<typename TVal>
        void Read(vector<TVal> &val) {
            size_t size;
            val.clear();
            Read(size);
            val.reserve(size);
            for (size_t i = 0; i < size; i++) {
                TVal newitem;
                Read(newitem);
                val.push_back(move(newitem));
            }
        }

        template<typename TA, typename TB>
        void Write(const pair<TA, TB> &val) {
            Write(val.first);
            Write(val.second);
        }

        template<typename TA, typename TB>
        void Read(pair<TA, TB> &val) {
            Read(val.first);
            Read(val.second);
        }

        template<typename TKey, typename TVal>
        void Write(const unordered_map<TKey, TVal> &val) {
            Write(val.size());
            for (const auto& item : val) {
                Write(item.first);
                Write(item.second);
            }
        }

        template<typename TKey, typename TVal>
        void Read(unordered_map<TKey, TVal> &val) {
            size_t size;
            val.clear();
            Read(size);
            for (size_t i = 0; i < size; i++) {
                TKey nk;
                Read(nk);
                Read(val[nk]);
            }
        }

        void Write(const Movie& val) {
            Write(val.rating_);
            Write(val.votes_);
            Write(val.year_);
            Write(val.type_);
            Write(val.subtitles_);
            Write(val.actors_);
            Write(val.genres_);
            Write(val.directors_);
            Write(val.composer_);
            Write(val.countries_);
            Write(val.languages_);
            Write(val.plots_);
            Write(val.length_);
        }

        void Read(Movie& val) {
            Read(val.rating_);
            Read(val.votes_);
            Read(val.year_);
            Read(val.type_);
            Read(val.subtitles_);
            Read(val.actors_);
            Read(val.genres_);
            Read(val.directors_);
            Read(val.composer_);
            Read(val.countries_);
            Read(val.languages_);
            Read(val.plots_);
            Read(val.length_);
        }

        void Write(const ReverseIndex& val) {
            Write(val.rev_idx_);
            Write(val.idx_);
        }

        void Read(ReverseIndex& val) {
            Read(val.rev_idx_);
            Read(val.idx_);
        }

        template<typename TKey, typename TVal>
        void Write(const Index<TKey, TVal>& val) {
            Write(val.k2id_);
            Write(val.id2val_);
        }

        template<typename TKey, typename TVal>
        void Read(Index<TKey, TVal>& val) {
            Read(val.k2id_);
            Read(val.id2val_);
        }

        void Write(const People& val) {
            Write(val.movies_);
        }

        void Read(People& val) {
            Read(val.movies_);
        }

    };

}

#endif


