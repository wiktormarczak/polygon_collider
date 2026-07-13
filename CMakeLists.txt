cmake_minimum_required(VERSION 4.0)

project(polygon_collider)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")

add_subdirectory(lib/SDL EXCLUDE_FROM_ALL)

add_executable(polygon_collider WIN32
    src/system/main.c src/core/app.c src/system/window.c src/graphics/renderer.c src/graphics/shader.c src/core/polygon_object.c src/graphics/color.c src/graphics/camera.c src/system/stopwatch.c src/geometry/vector.c src/graphics/batch.c src/geometry/line.c src/geometry/edge.c src/core/vector_object.c src/core/vector_object_queue.c src/geometry/polygon.c lib/glad/src/gl.c)
target_include_directories(polygon_collider PRIVATE include lib/glad/include)
target_link_libraries(polygon_collider PRIVATE SDL3::SDL3)

if(NOT WIN32)
    target_link_libraries(polygon_collider PRIVATE m)
endif()

add_custom_command(TARGET polygon_collider POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/glsl/ $<TARGET_FILE_DIR:polygon_collider>/glsl/)
