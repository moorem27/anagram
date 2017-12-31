cmake_minimum_required( VERSION 3.5 )
project( anagram )

set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -O2" )

add_executable( anagram anagram.cc )
