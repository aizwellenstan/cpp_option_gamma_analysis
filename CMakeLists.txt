cmake_minimum_required(VERSION 3.13.4)

set(CMAKE_CXX_FLAGS "-std=c++17")

project(CMAKE)

add_executable(${PROJECT_NAME} main.cpp)

add_subdirectory(modules)

target_include_directories(${PROJECT_NAME} PRIVATE modules)

target_link_directories(${PROJECT_NAME} PRIVATE modules)

target_link_libraries(${PROJECT_NAME} modules)