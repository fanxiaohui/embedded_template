include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/
    ${CMAKE_CURRENT_SOURCE_DIR}/drivers
    ${CMAKE_CURRENT_SOURCE_DIR}/platform_os
)

set(SOURCES "")
set(TEST_SOURCES "")

add_subdirectory(async)
add_subdirectory(drivers)
add_subdirectory(i2c)
add_subdirectory(spi)
add_subdirectory(gpio)
add_subdirectory(misc)
add_subdirectory(ringbuffer)

add_library(
    embedded
    ${SOURCES}
)
