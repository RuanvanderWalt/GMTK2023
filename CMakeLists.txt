#file(COPY assets DESTINATION .)

cmake_minimum_required(VERSION 3.26)
project(gmtk2023)

include_directories("/usr/local/include")

find_package(raylib 3.0 REQUIRED)

set(CMAKE_CXX_STANDARD 17)

add_executable(gmtk2023 main.cpp)

target_link_libraries(gmtk2023 raylib)
