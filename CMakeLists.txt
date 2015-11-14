cmake_minimum_required(VERSION 2.8)

set(CMAKE_BUILD_TYPE DEBUG)
set(CMAKE_C_FLAGS "-O0 -ggdb")
set(CMAKE_C_FLAGS_DEBUG "-O0 -ggdb")
set(CMAKE_C_FLAGS_RELEASE "-O0 -ggdb")
set(CMAKE_CXX_FLAGS "-O0 -ggdb")
set(CMAKE_CXX_FLAGS_DEBUG "-O0 -ggdb")
set(CMAKE_CXX_FLAGS_RELEASE "-O0 -ggdb")


project(embedded_template)

include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/Unity/src
    ${CMAKE_CURRENT_SOURCE_DIR}/Unity/extras/fixture/src/
)

add_library(
    unity
    Unity/src/unity.c
    Unity/extras/fixture/src/unity_fixture.c
)

add_subdirectory(src)

