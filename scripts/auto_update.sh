#!/bin/sh

echo "./auto_update.sh filelist.txt outputdir db_build.exec output.file"
echo "example: ./auto_update.sh filelist.txt /disk2/imdb/ ~/IMDB_predictor/src/db_build temp.dat"
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
        curl -o $2/${i}.list.gz "ftp://ftp.funet.fi/pub/mirrors/ftp.imdb.com/pub/${i}.list.gz"
        gunzip -f $2/${i}.list.gz
        newdata=1
        mv tmp $2/${i}.info
    fi
done
rm -f tmp

if [ $newdata -eq 1 ]
then
#Run the parsers
$3 $4 $2
fi

