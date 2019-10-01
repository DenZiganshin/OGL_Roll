cmake_minimum_required(VERSION 3.7)

project(roll)

set(HEADER_FILES utils.h)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall")
add_definitions(-DNO_TTF -DOGL)

find_package(SDL2 REQUIRED)

add_executable(${PROJECT_NAME} main.cpp utils.cpp ${HEADER_FILES})
target_link_libraries(${PROJECT_NAME} ${SDL2_LIBRARIES} GL GLU)
