project("embedded_template")


set(CMAKE_C_FLAGS "-D__FAR=\"\" -DSHELL_SUPPORT_HELP=1")

add_subdirectory(src)
