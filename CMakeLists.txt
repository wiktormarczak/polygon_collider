cmake_minimum_required(VERSION 4.0)

project(polygon_collider)

set(CMAKE_C_STANDARD 90)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")

add_subdirectory(lib/SDL EXCLUDE_FROM_ALL)

add_executable(polygon_collider WIN32 src/main.c src/app.c src/window.c src/renderer.c src/shader.c src/polygon.c src/vertex_buffer.c src/color.c src/camera.c src/timer.c src/vector.c lib/glad/src/gl.c)
target_include_directories(polygon_collider PRIVATE include lib/glad/include)
target_link_libraries(polygon_collider PRIVATE SDL3::SDL3 m)
