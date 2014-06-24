#!/bin/sh

#OPT=-O4
DEBUG=-g
#SPARSEHASH=-DUSE_SPARSE_HASH


CFLAGS=-std=c++11 -Wall -Wextra -Wno-format -fPIC $(OPT) $(DEBUG) $(SPARSEHASH)
CC=g++
LIBS=-ltws -lz
INC_PATH=-I./
LIB_PATH=-L./

OBJS=DataSchema.o MovieDB.o Log.o Index.o Common.o
SERVEROBJS=server_main.o HttpHandler.o DynamicHTML.o ZlibCompression.o
PARSEROBJS=Parser.o parsers/actors.o \
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

all: server db_build

server: $(OBJS) $(SERVEROBJS)
	$(CC) -o $@ $^ $(LIBS) $(LIB_PATH)
	
parser_test: $(OBJS) $(PARSEROBJS) $(PARSERTESTOBJ)
	$(CC) -o $@ $^ $(LIB_PATH)

db_build: $(OBJS) $(PARSEROBJS) $(DBBUILDOBJ)
	$(CC) -o $@ $^ $(LIB_PATH)

%.o: %.cpp
	$(CC) $(INC_PATH) -c $(CFLAGS) -o $@ $^

clean:
	rm -f *.o parsers/*.o server parser_test

rebuild: clean all



