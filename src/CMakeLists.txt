
include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/
    ${CMAKE_CURRENT_SOURCE_DIR}/../third_party/FreeRTOS/FreeRTOS/
    ${CMAKE_CURRENT_SOURCE_DIR}/../third_party/FreeRTOS/FreeRTOS/Source/include
    ${CMAKE_CURRENT_SOURCE_DIR}/../third_party/FreeRTOS/FreeRTOS/Source/portable/MSVC-MingW
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


