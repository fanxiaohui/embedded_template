add_library(
    async
    async.c
    looper.c
    event.c
    platform_posix.c
)


set(SOURCES
    ${SOURCES}
    ${CMAKE_CURRENT_SOURCE_DIR}/async.c
    ${CMAKE_CURRENT_SOURCE_DIR}/event.c
    PARENT_SCOPE
)

