
include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/
)

add_subdirectory(async)
add_subdirectory(ringbuffer)
add_subdirectory(drivers)

add_executable(
    async_example
    async_example.c
)
target_link_libraries(
    async_example
    async
    ringbuffer
    pthread
    m
)


add_executable(
    unit_test 
    unit_test.c
)
target_link_libraries(
    unit_test
    ringbuffer
    unity
    drivers
)


