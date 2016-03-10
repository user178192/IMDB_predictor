#!/bin/sh

cd $1

for i in `cat ./scripts/filelist.txt`
do
#    curl -o $2/${i}.list.gz "ftp://ftp.funet.fi/pub/mirrors/ftp.imdb.com/pub/${i}.list.gz"
#    gunzip -f $2/${i}.list.gz
    curl -O "ftp://ftp.funet.fi/pub/mirrors/ftp.imdb.com/pub/${i}.list.gz"
done
