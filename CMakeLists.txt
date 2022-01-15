cmake_minimum_required(VERSION 3.15)

project(SparseDomainPartitioningForLSB)

find_package(Boost COMPONENTS program_options REQUIRED)

include_directories(include)

set(CMAKE_CXX_FLAGS "-std=c++14 -g -Wall -Wextra -Wno-unused-parameter")

file(GLOB SOURCES "src/*.cpp")
add_executable(SparseDomainPartitioningForLSB ${SOURCES})

target_link_libraries(SparseDomainPartitioningForLSB ${Boost_LIBRARIES})
