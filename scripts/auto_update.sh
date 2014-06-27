#!/bin/sh

newdata=0

for i in `cat $1`
do
    curl  -I "ftp://ftp.funet.fi/pub/mirrors/ftp.imdb.com/pub/${i}.list.gz" > tmp
    diff tmp $2/${i}.info
    if [ $? -eq 0 ]
    then
        echo "same"
    else
        echo "Updating ${i}.list....."
        curl -o ${i}.list.gz "ftp://ftp.funet.fi/pub/mirrors/ftp.imdb.com/pub/${i}.list.gz"
        gunzip -f ${i}.list.gz
        newdata=1
        mv tmp $2/${i}.info
    fi
done
rm -f tmp

if [ $newdata -eq 1 ]
#Run the parsers
fi

