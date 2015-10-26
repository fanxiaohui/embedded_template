include_directories(.)

add_subdirectory(async)
add_subdirectory(ringbuffer)

add_executable(aa main.c)
target_link_libraries(
    aa
    async
    ringbuffer

    pthread
    m
)


