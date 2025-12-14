
add_executable(main_linking main_linking.c)

target_link_libraries(main_linking
    PRIVATE
        math_lib
        sort_lib
)

set_target_properties(main_linking PROPERTIES
    BUILD_RPATH "${CMAKE_LIBRARY_OUTPUT_DIRECTORY}"
)

add_executable(main_dynamic main_dynamic.c)

target_link_libraries(main_dynamic
    PRIVATE
        dl
)

set_target_properties(main_dynamic PROPERTIES
    BUILD_RPATH "${CMAKE_LIBRARY_OUTPUT_DIRECTORY}"
)

message(STATUS "build in ${CMAKE_BUILD_RPATH}")
