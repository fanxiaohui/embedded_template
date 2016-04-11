get_filename_component(APP_NAME ${CMAKE_CURRENT_SOURCE_DIR} NAME)
file(GLOB APP_SRC "*.h" "*.c" "*.cpp")

add_executable(
    ${APP_NAME}
    ${APP_SRC}
    ${APP_EXT_SRC}
)

target_link_libraries(
    ${APP_NAME}
    embedded
)


if (NOT (PLATFORM_OS STREQUAL "posix"))

add_custom_target(
    ${APP_NAME}.bin
    ALL
    COMMAND ${OBJCOPY} -O binary ${APP_NAME} ${APP_NAME}.bin
    DEPENDS
    ${APP_NAME}
)

add_custom_target(
    ${APP_NAME}.map
    ALL
    COMMAND ${OBJDUMP} -x ${APP_NAME} >${APP_NAME}.map
    DEPENDS
    ${APP_NAME}
)

endif()
