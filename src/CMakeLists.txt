include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/
    ${CMAKE_CURRENT_SOURCE_DIR}/drivers
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

add_library(
    embedded_test
    ${TEST_SOURCES}
)


add_executable(
    unit_test 
    unit_test.c
)
target_link_libraries(
    unit_test
    embedded_test
    embedded
    unity
)

