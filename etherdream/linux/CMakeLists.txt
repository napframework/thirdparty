cmake_minimum_required(VERSION 3.5)

enable_language(C)

set(CMAKE_THREAD_PREFER_PTHREAD TRUE)
set(THREADS_PREFER_PTHREAD_FLAG TRUE)
find_package(Threads REQUIRED)

include_directories(src/common)

add_library(etherdream SHARED src/driver/libetherdream/etherdream.c)
target_link_libraries(etherdream m Threads::Threads)