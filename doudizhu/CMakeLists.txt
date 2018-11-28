cmake_minimum_required(VERSION 2.8)

project(doudizhu_endgame)

set(CMAKE_CXX_STANDARD 14)

SET(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -O0 -Wall -g")
SET(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} -O3 -Wall")

#SET(CMAKE_BUILD_TYPE "Debug")
SET(CMAKE_BUILD_TYPE "Release")

aux_source_directory(. all_source_file)

add_executable(doudizhu_endgame ${all_source_file})