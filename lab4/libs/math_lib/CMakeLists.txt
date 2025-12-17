set(MATH_LIB_SOURCES
    math_lib.c
)
set(MATH_LIB_HEADERS
    math_lib.h
)
add_library(math_lib SHARED
    ${MATH_LIB_SOURCES}
    ${MATH_LIB_HEADERS}
)

target_link_libraries(math_lib PRIVATE m)

target_include_directories(math_lib
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)
