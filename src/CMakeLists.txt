
include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/
)

add_subdirectory(async)
add_subdirectory(ringbuffer)

add_executable(
    aa
    main.c
)
target_link_libraries(
    aa
    async
    ringbuffer
    pthread
    m
)


