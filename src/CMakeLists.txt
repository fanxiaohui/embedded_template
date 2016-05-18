include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/
    ${CMAKE_CURRENT_SOURCE_DIR}/drivers
    ${CMAKE_CURRENT_SOURCE_DIR}/platform_os
    ${CMAKE_CURRENT_SOURCE_DIR}/shell
    ${CMAKE_CURRENT_SOURCE_DIR}/ringbuffer
    ${CMAKE_CURRENT_SOURCE_DIR}/atcmd
    ${CMAKE_CURRENT_SOURCE_DIR}/misc
    ${CMAKE_CURRENT_SOURCE_DIR}/dtu_m35
    ${CMAKE_CURRENT_SOURCE_DIR}/mqtt/MQTTClient-C/src
    ${CMAKE_CURRENT_SOURCE_DIR}/mqtt/MQTTPacket/src

)

set(SOURCES "")
set(TEST_SOURCES "")

add_subdirectory(async)
add_subdirectory(drivers)
add_subdirectory(i2c)
add_subdirectory(spi)
add_subdirectory(gpio)
add_subdirectory(misc)
add_subdirectory(dtu_m35)
add_subdirectory(shell)
add_subdirectory(ringbuffer)
add_subdirectory(atcmd)
add_subdirectory(platform_os)
add_subdirectory(mqtt)

add_library(
    embedded
    STATIC
    ${SOURCES}
)

add_subdirectory(example)
add_subdirectory(application)
