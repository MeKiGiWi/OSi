set(MATH_LIB_SOURCES
)
set(MATH_LIB_HEADERS
)
add_library(math_lib SHARED
    math_lib.c
    math_lib.h
)

target_link_libraries(math_lib PRIVATE m)

target_include_directories(math_lib
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)
