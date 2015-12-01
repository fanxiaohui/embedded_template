include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/
    ${CMAKE_CURRENT_SOURCE_DIR}/softspi
)

set(
    SPI_SOURCE
    spi.c
    softspi/softspi.c
)

add_library(
    spi
    ${SPI_SOURCE}
)

if(COMMAND add_pc_lint)
    add_pc_lint(spi ${SPI_SOURCE})
endif(COMMAND add_pc_lint)

