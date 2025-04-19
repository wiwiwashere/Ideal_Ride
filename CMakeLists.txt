cmake_minimum_required(VERSION 3.14)
project(IdealRide)

set(CMAKE_CXX_STANDARD 17)

# find the CURL package
find_package(CURL REQUIRED)

# your executable
add_executable(ride_planner
        src/main.cpp
        src/Graph.cpp
)

# include the CURL headers
target_include_directories(ride_planner
        PRIVATE
        ${CURL_INCLUDE_DIR}
)

# link against libcurl
target_link_libraries(ride_planner
        PRIVATE
        ${CURL_LIBRARIES}   # usually this expands to -lcurl
)

target_include_directories(ride_planner
        PRIVATE
        ${PROJECT_SOURCE_DIR}/include
)