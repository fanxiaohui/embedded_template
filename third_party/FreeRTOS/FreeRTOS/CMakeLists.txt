include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/
    ${CMAKE_CURRENT_SOURCE_DIR}/Source/include
    ${CMAKE_CURRENT_SOURCE_DIR}/Source/portable/MSVC-MingW
)

add_library(
    freertos
    Source/croutine.c
    Source/event_groups.c
    Source/list.c
    Source/queue.c
    Source/tasks.c
    Source/timers.c
    Source/portable/MSVC-Mingw/port.c
    Source/portable/MemMang/heap_5.c
)

