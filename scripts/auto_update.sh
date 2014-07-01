#!/bin/sh

#/etc/crontab run 3am every day
#movie_predictor auto data download and server restart
#0 3     * * *   ubuntu  date >> /home/ubuntu/update.log; /home/ubuntu/IMDB_predictor/scripts/auto_update.sh /home/ubuntu/IMDB_predictor/ /data/imdb/ >> /home/ubuntu/imdb_update.log

echo "./auto_update.sh imdb_working_path data_path"
echo "example: ./auto_update.sh ~/IMDB_predictor/ /disk2/imdb"
newdata=0

cd $1

for i in `cat ./scripts/filelist.txt`
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
	#prevent insufficient memory, kill current running instance first

	echo "New Data found, have to kill running server"
	sudo kill `ps aux|grep imdb_server | grep -v -E "grep|nohup" | awk '{print $2}'`

	echo "killed, begin to build"
	./build/imdb_db_build imdb_predictor.dat $2

	echo "data built"
	sudo nohup ./build/imdb_server 80 $2/imdb_predictor.dat ./html/ &

	echo "server started again"
fi


