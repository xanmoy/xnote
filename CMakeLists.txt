cmake_minimum_required(VERSION 3.10)
project(xnote C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall")

find_package(PkgConfig REQUIRED)
pkg_check_modules(GTK3 REQUIRED gtk+-3.0)

include_directories(${GTK3_INCLUDE_DIRS})
link_directories(${GTK3_LIBRARY_DIRS})

add_executable(xnote xnote.c)

target_link_libraries(xnote ${GTK3_LIBRARIES})

# Install target
install(TARGETS xnote DESTINATION bin)
