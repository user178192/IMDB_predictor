#!/bin/sh

#OPT=-O4
DEBUG=-g

CFLAGS=-std=c++11 -Wall -Wextra -Wno-format -fPIC $(OPT) $(DEBUG) 
CC=g++
LIBS=-ltws
INC_PATH=-I./
LIB_PATH=-L./

OBJS=DataSchema.o MovieDB.o Log.o
SERVEROBJS=server_main.o
PARSEROBJS=Parser.o parsers/actors.o parsers/movies.o parsers/languages.o parsers/genres.o parsers/countries.o
DBBUILDOBJ=db_build.o
PARSERTESTOBJ=parser_test.o

all: server db_build

server: $(OBJS) $(SERVEROBJS)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS) $(LIB_PATH)
	
parser_test: $(OBJS) $(PARSEROBJS) $(PARSERTESTOBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LIB_PATH)

db_build: $(OBJS) $(PARSEROBJS) $(DBBUILDOBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LIB_PATH)

%.o: %.cpp
	$(CC) $(INC_PATH) -c $(CFLAGS) -o $@ $^

clean:
	rm -f *.o parsers/*.o server parser

rebuild: clean all



