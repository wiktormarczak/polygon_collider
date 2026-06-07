cmake_minimum_required(VERSION 4.0)

project(polygon_collider)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")

add_subdirectory(lib/SDL EXCLUDE_FROM_ALL)

add_executable(polygon_collider WIN32 src/main.c lib/glad/src/gl.c)
target_include_directories(polygon_collider PRIVATE lib/glad/include)
target_link_libraries(polygon_collider PRIVATE SDL3::SDL3)
