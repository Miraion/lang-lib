#!/bin/bash

echo creating c++17 library...
g++ -std=c++1z -c lang/src/*.cpp
ar rvs lang17.a *.o

echo creating c++14 library...
g++ -std=c++14 -c lang/src/*.cpp
ar rvs lang14.a *.o

echo cleaning...
rm *.o

echo done
