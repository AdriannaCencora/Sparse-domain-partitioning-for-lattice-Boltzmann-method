cmake_minimum_required(VERSION 3.15)

project(SparseDomainPartitioningForLSB)

include_directories(include)

set (CMAKE_CXX_FLAGS "-std=c++14 -g -Wall -Werror -Wextra -Wno-unused-parameter")

file(GLOB SOURCES "src/*.cpp")
add_executable(SparseDomainPartitioningForLSB ${SOURCES})
