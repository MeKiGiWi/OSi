set(SORT_LIB_SOURCES
    sort_lib.c
)
set(SORT_LIB_HEADERS
    sort_lib.h
)
add_library(sort_lib SHARED
    ${SORT_LIB_SOURCES}
    ${SORT_LIB_HEADERS}
)


target_include_directories(sort_lib
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)
