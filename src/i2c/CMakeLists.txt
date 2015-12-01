include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/
)

set(
    I2C_SOURCE
    i2c.c
    softi2c/softi2c.c
)

add_library(
    i2c
    ${I2C_SOURCE}
)

if(COMMAND add_pc_lint)
    add_pc_lint(i2c ${SPI_SOURCE})
endif(COMMAND add_pc_lint)

