#!/bin/sh

#OPT=-O4
DEBUG=-g

CFLAGS=-std=c++11 -Wall -Wextra -pedantic -Wno-format -fPIC $(OPT) $(DEBUG) 
CC=g++
LIBS=-ltws
INC_PATH=-I./
LIB_PATH=-L./

OBJS=DataSchema.o MovieDB.o Log.o
SERVEROBJS=server_main.o
PARSEROBJS=Parser.o

all: server parser

server: $(OBJS) $(SERVEROBJS)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS) $(LIB_PATH)
	
parser: $(OBJS) $(PARSEROBJS)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS) $(LIB_PATH)

%.o: %.cpp
	$(CC) $(INC_PATH) -c $(CFLAGS) -o $@ $^

clean:
	rm -f *.o server parser

rebuild: clean all



