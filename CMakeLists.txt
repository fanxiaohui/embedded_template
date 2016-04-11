cmake_minimum_required(VERSION 2.8)

project("embedded_template")

set(TOPDIR ${CMAKE_CURRENT_SOURCE_DIR})

if (NOT ENV{PLATFORM_OS})
    set(PLATFORM_OS "posix")
else()
    set(PLATFORM_OS $ENV{PLATFORM_OS})
endif()
message("set PLATFORM_OS=\"${PLATFORM_OS}\"")

set(AppBuild "${TOPDIR}/cmake/app_build.cmake")

include(cmake/${PLATFORM_OS}.cmake)

add_subdirectory(src)
