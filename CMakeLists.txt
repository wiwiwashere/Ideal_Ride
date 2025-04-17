cmake_minimum_required(VERSION 3.10)
project(RidePlanner)

set(CMAKE_CXX_STANDARD 17)

# Add executable
add_executable(ride_planner
    src/main.cpp
    src/Graph.cpp
)

# Include directories
target_include_directories(ride_planner PRIVATE include)