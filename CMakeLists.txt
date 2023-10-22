cmake_minimum_required(VERSION 3.27.7)
project(bds VERSION 0.0.1 LANGUAGES C CXX)

find_package(LLVM REQUIRED CONFIG)
include_directories(${LLVM_INCLUDE_DIRS})
add_definitions(${LLVM_DEFINITIONS})

llvm_map_components_to_libnames(llvm_libs support core irreader)

file(GLOB_RECURSE SOURCES CONFIFURE_DEPENDS "src/*.cpp")

add_executable(bds ${SOURCES})
target_compile_features(bds PRIVATE cxx_std_23)
target_include_directories(bds PRIVATE include)
target_link_libraries(bds PRIVATE ${llvm_libs})

install(TARGETS bds)