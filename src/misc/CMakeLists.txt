include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/
)

set(
    MISC_SOURCE
    second_datetime.c
)

add_library(
    misc
    ${MISC_SOURCE}
)

if(COMMAND add_pc_lint)
    add_pc_lint(misc ${MISC_SOURCE})
endif(COMMAND add_pc_lint)

