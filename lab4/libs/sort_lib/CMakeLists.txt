add_library(sort_lib SHARED
    sort_lib.c
    sort_lib.h
)


target_include_directories(sort_lib
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)
