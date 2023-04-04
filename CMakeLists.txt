cmake_minimum_required(VERSION 3.0.0)

project(ban_ip)

set(CMAKE_BUILD_TYPE "Debug")

include_directories(include)

aux_source_directory(src include_src)

add_executable(main main.cpp ${include_src})

add_compile_options(-std=c++11)

target_link_libraries(main pthread)


