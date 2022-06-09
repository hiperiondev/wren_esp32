file(
    GLOB_RECURSE
        SOURCES
            vm/*.c
            optional/*.c
)

idf_component_register(
    SRCS ${SOURCES}
    INCLUDE_DIRS
        include/
        vm/
        optional/
        .
    REQUIRES
        fatfs
        sdmmc
)