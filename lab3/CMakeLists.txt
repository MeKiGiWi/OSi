cmake_minimum_required(VERSION 3.10)
project(lab1)

add_executable(child src/child.c)
add_executable(main src/main.c)

# Add required libraries for shared memory and semaphores
target_link_libraries(child rt pthread)
target_link_libraries(main rt pthread)
