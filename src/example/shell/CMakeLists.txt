add_executable(
    shell
    main.c
    shell_config.c
)

target_link_libraries(
    shell
    embedded
)