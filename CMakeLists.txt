cmake_minimum_required(VERSION 3.13.4)

project(CMAKE)

# Set C++ standard to C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Set build type to Release (this is the optimized build type)
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE "Release")
endif()

# Create an executable target
add_executable(${PROJECT_NAME} main.cpp)

# Add a subdirectory for modules
add_subdirectory(modules)

# Include and link modules to the main target
target_include_directories(${PROJECT_NAME} PRIVATE modules)
target_link_libraries(${PROJECT_NAME} PRIVATE modules)

# Set compiler-specific flags for performance optimization
if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
  # For GCC (G++) compiler
  target_compile_options(${PROJECT_NAME} PRIVATE "-O3")  # Maximum optimization level
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
  # For Clang compiler
  target_compile_options(${PROJECT_NAME} PRIVATE "-O3")  # Maximum optimization level
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
  # For Visual Studio compiler
  target_compile_options(${PROJECT_NAME} PRIVATE "/O2")  # Maximum optimization level
endif()
