cmake_minimum_required(VERSION 3.28.0)
project(topo-sort)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add include directories
include_directories(include)

# Add source files
file(GLOB MAIN_SOURCES "src/main.cpp")
file(GLOB SOURCES "src/AdjacencyList.cpp" "src/Graph.cpp")
file(GLOB TEST_SOURCES "tests/*.cpp")

# Add executables
add_executable(topo-sort ${SOURCES} ${MAIN_SOURCES})
add_executable(topo-test ${TEST_SOURCES} ${SOURCES})

# Add tests
enable_testing()
add_test(NAME topo-test COMMAND topo-test)
