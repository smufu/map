cmake_minimum_required(VERSION 3.0)
project("map")
add_definitions("-std=c++14 -Wall -pedantic")
add_executable(map ./main.cpp ./map.cpp )
