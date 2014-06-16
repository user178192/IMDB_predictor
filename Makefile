#!/bin/sh

all: reader.cpp reader.hpp
	g++  -std=c++11 -Wall -Wextra -pedantic reader.cpp  -O2
clean:
	rm reader 
