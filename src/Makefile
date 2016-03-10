#!/bin/sh

BUILDDIR=../build/
OPT=-O3
DEBUG=-g -D_LOG_NONE
#SPARSEHASH=-DUSE_SPARSE_HASH


CFLAGS=-std=c++11 -Wall -Wextra -Wno-format -fPIC $(OPT) $(DEBUG) $(SPARSEHASH)
CC=g++
LIBS=-ltws
INC_PATH=-I./
LIB_PATH=-L../libs/

COMMONOBJS=common/DataSchema.o common/MovieDB.o common/Log.o common/Index.o common/Common.o
SERVEROBJS=server_main.o server/HttpHandler.o server/DynamicHTML.o relation/MovieRelation.o
PARSEROBJS=parsers/Parser.o parsers/actors.o \
					parsers/directors.o \
					parsers/movies.o \
					parsers/languages.o \
					parsers/composers.o \
					parsers/genres.o \
					parsers/countries.o \
					parsers/ratings.o \
					parsers/runningtime.o

DBBUILDOBJ=db_build.o
PARSERTESTOBJ=parser_test.o
RELATIONOBJ=relation/MovieRelation.o relation_build.o

all: $(BUILDDIR) $(BUILDDIR)imdb_server $(BUILDDIR)imdb_db_build $(BUILDDIR)imdb_relation_build $(BUILDDIR)parser_test

#parser_test: $(BUILDDIR) $(BUILDDIR)parser_test

#relation: $(BUILDDIR) $(BUILDDIR)imdb_relation_build

$(BUILDDIR): 
	mkdir -p $(BUILDDIR)

$(BUILDDIR)imdb_server: $(COMMONOBJS) $(SERVEROBJS)
	$(CC) -o $@ $^ $(LIBS) $(LIB_PATH)
	
$(BUILDDIR)parser_test: $(COMMONOBJS) $(PARSEROBJS) $(PARSERTESTOBJ)
	$(CC) -o $@ $^ $(LIB_PATH)

$(BUILDDIR)imdb_relation_build: $(COMMONOBJS) $(RELATIONOBJ)
	$(CC) -o $@ $^ $(LIB_PATH)

$(BUILDDIR)imdb_db_build: $(COMMONOBJS) $(PARSEROBJS) $(DBBUILDOBJ)
	$(CC) -o $@ $^ $(LIB_PATH)

%.o: %.cpp
	$(CC) $(INC_PATH) -c $(CFLAGS) -o $@ $^

clean:
	rm -f *.o parsers/*.o server/*.o common/*.o relation/*.o $(BUILDDIR)imdb* $(BUILDDIR)parser_test

rebuild: clean all



