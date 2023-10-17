cmake_minimum_required(VERSION 3.27.7)
project(bds VERSION 0.0.1 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(LLVM REQUIRED CONFIG)

file(GLOB_RECURSE SOURCES CONFIFURE_DEPENDS "src/*.cpp")

add_executable(bds ${SOURCES})
target_include_directories(bds PRIVATE include)
target_link_libraries(bds PRIVATE LLVM)

install(TARGETS bds)