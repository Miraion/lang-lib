#!/bin/bash

link="$1"

if [[ $link == "-l" ]]
then
    echo compiling library...
    ./mklib.sh
    echo compiling with library linkage...
    g++ -std=c++1z -c test.cpp
    g++ -o test test.o lang17.a
    rm test.o
else
    echo compiling without library linkage...
    g++ -std=c++1z -o test test.cpp
fi
echo done
