cmake_minimum_required(VERSION 3.15)

project(SparseDomainPartitioningForLSB)

find_package(Boost COMPONENTS program_options REQUIRED)

include_directories(include)
add_subdirectory(nlohmann_json)

set(CMAKE_CXX_FLAGS "-std=c++14 -g -Wall -Wextra -Wno-unused-parameter")

file(GLOB SOURCES "src/*.cpp")
add_executable(SparseDomainPartitioningForLSB ${SOURCES})

target_link_libraries(SparseDomainPartitioningForLSB PRIVATE ${Boost_LIBRARIES} nlohmann_json::nlohmann_json)
