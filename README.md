IMDB_Predictor
============

Currently runs on website [7Movie.me][website]

IMDB_Predictor

Try to build a simple machine learning program to predict new movie's IMDB rating according to the IMDB database.  
How to compile and run  
    1. Execute makefile  
    2. Go to build directory and execute following execute files in turns:  
        ./imdb_db_build filename.db rawfile.dir  
        ./imdb_relation_build filename.db  
        ./imdb_server port filename.db html_template_dir  
    3. Open the browser and access the html page to do searching  
        If you want to test local, please use url: 127.0.0.1:port  

Enjoy!


[website]:http://7movie.me/
