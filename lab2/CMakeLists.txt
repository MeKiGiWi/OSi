cmake_minimum_required(VERSION 3.10)
project(lab1)

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

include_directories(include)
add_executable(main src/main.c src/sort.c src/utils.c)
target_link_libraries(main pthread)
# target_link_libraries(main Threads::Threads)

