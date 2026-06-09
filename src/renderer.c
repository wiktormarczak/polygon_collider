/* Copyright 2026 Wiktor Marczak 
 * This file is part of Polygon Collider. */

/* Polygon Collider is free software: you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation, either version 3 of the
 * License, or (at your option) any later version. */

/* Polygon Collider is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details. */

/* You should have received a copy of the GNU General Public License
 * along with Polygon Collider. If not, see <https://www.gnu.org/licenses/>. */

#define VERTEX_BUFFER_SIZE 100
#define INDEX_BUFFER_SIZE 300
#define VECTOR_VERTEX_BUFFER_SIZE 50
#define VECTOR_INDEX_BUFFER_SIZE 100

#include <polygon_collider/renderer.h>
#include <polygon_collider/shader.h>
#include <polygon_collider/camera.h>
#include <polygon_collider/polygon.h>
#include <polygon_collider/vector.h>
#include <polygon_collider/color.h>

#include <glad/gl.h>

#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

struct Renderer
{
    unsigned int shader_program;
    unsigned int matrix_location;

    /* Polygons */
    unsigned int vertex_array;

    unsigned int vertex_position_buffer, vertex_color_buffer, index_buffer;

    unsigned int vertex_count, index_count;
    Vector vertex_position_data[VERTEX_BUFFER_SIZE];
    Color vertex_color_data[VERTEX_BUFFER_SIZE];
    unsigned int index_data[INDEX_BUFFER_SIZE];

    /* Vectors */
    unsigned int vector_vertex_array;

    unsigned int vector_vertex_position_buffer, vector_vertex_color_buffer, vector_index_buffer;

    unsigned int vector_vertex_count, vector_index_count;
    Vector vector_vertex_position_data[VECTOR_VERTEX_BUFFER_SIZE];
    Color vector_vertex_color_data[VECTOR_VERTEX_BUFFER_SIZE];
    unsigned int vector_index_data[VECTOR_INDEX_BUFFER_SIZE];

    float matrix[16];
};

Renderer *renderer_create()
{
    Renderer *renderer = malloc(sizeof(Renderer));

    renderer->shader_program = shader_create_program("glsl/vertex_shader.glsl", "glsl/fragment_shader.glsl");
    renderer->matrix_location = glGetUniformLocation(renderer->shader_program, "matrix");

    glUseProgram(renderer->shader_program);

    /* Polygons */
    glGenVertexArrays(1, &renderer->vertex_array);
    glGenBuffers(1, &renderer->vertex_position_buffer);
    glGenBuffers(1, &renderer->vertex_color_buffer);
    glGenBuffers(1, &renderer->index_buffer);

    glBindVertexArray(renderer->vertex_array);

    glBindBuffer(GL_ARRAY_BUFFER, renderer->vertex_position_buffer);
    glBufferData(GL_ARRAY_BUFFER, VERTEX_BUFFER_SIZE * sizeof(Vector), NULL, GL_DYNAMIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, false, 2 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, renderer->vertex_color_buffer);
    glBufferData(GL_ARRAY_BUFFER, VERTEX_BUFFER_SIZE * sizeof(Color), NULL, GL_DYNAMIC_DRAW);
    glVertexAttribPointer(1, 3, GL_FLOAT, false, 3 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, renderer->index_buffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, INDEX_BUFFER_SIZE * sizeof(unsigned int), NULL, GL_DYNAMIC_DRAW);

    renderer->vertex_count = 0;
    renderer->index_count = 0;

    /* Vectors */
    glGenVertexArrays(1, &renderer->vector_vertex_array);
    glGenBuffers(1, &renderer->vector_vertex_position_buffer);
    glGenBuffers(1, &renderer->vector_vertex_color_buffer);
    glGenBuffers(1, &renderer->vector_index_buffer);

    glBindVertexArray(renderer->vector_vertex_array);

    glBindBuffer(GL_ARRAY_BUFFER, renderer->vector_vertex_position_buffer);
    glBufferData(GL_ARRAY_BUFFER, VECTOR_VERTEX_BUFFER_SIZE * sizeof(Vector), NULL, GL_DYNAMIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, false, 2 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, renderer->vector_vertex_color_buffer);
    glBufferData(GL_ARRAY_BUFFER, VECTOR_VERTEX_BUFFER_SIZE * sizeof(Color), NULL, GL_DYNAMIC_DRAW);
    glVertexAttribPointer(1, 3, GL_FLOAT, false, 3 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, renderer->vector_index_buffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, VECTOR_INDEX_BUFFER_SIZE * sizeof(unsigned int), NULL, GL_DYNAMIC_DRAW);

    renderer->vector_vertex_count = 0;
    renderer->vector_index_count = 0;

    return renderer;
}

void renderer_destroy(Renderer *renderer)
{
    glDeleteProgram(renderer->shader_program);
    renderer->shader_program = 0;
    renderer->matrix_location = 0;

    glDeleteVertexArrays(1, &renderer->vertex_array);
    renderer->vertex_array = 0;

    glDeleteBuffers(1, &renderer->vertex_position_buffer);
    renderer->vertex_position_buffer = 0;

    glDeleteBuffers(1, &renderer->vertex_color_buffer);
    renderer->vertex_color_buffer = 0;

    glDeleteBuffers(1, &renderer->index_buffer);
    renderer->index_buffer = 0;

    renderer->vertex_count = 0;
    renderer->index_count = 0;

    for(int i = 0; i < VERTEX_BUFFER_SIZE; i++)
        renderer->vertex_position_data[i].x = renderer->vertex_position_data[i].y = 0.0f;

    for(int i = 0; i < INDEX_BUFFER_SIZE; i++)
        renderer->index_data[i] = 0;

    glDeleteVertexArrays(1, &renderer->vector_vertex_array);
    renderer->vector_vertex_array = 0;

    glDeleteBuffers(1, &renderer->vector_vertex_position_buffer);
    renderer->vector_vertex_position_buffer = 0;

    glDeleteBuffers(1, &renderer->vector_vertex_color_buffer);
    renderer->vector_vertex_color_buffer = 0;

    glDeleteBuffers(1, &renderer->vector_index_buffer);
    renderer->vector_index_buffer = 0;

    renderer->vector_vertex_count = 0;
    renderer->vector_index_count = 0;

    for(int i = 0; i < VECTOR_VERTEX_BUFFER_SIZE; i++)
        renderer->vector_vertex_position_data[i].x = renderer->vector_vertex_position_data[i].y = 0.0f;

    for(int i = 0; i < VECTOR_INDEX_BUFFER_SIZE; i++)
        renderer->vector_index_data[i] = 0;

    free(renderer);
}

void renderer_submit_polygon(Renderer* renderer, Polygon *polygon)
{
    unsigned int initial_vertex = renderer->vertex_count;
    unsigned int initial_index = renderer->index_count;
    unsigned int delta_vertex_count = polygon_get_vertex_count(polygon);
    unsigned int delta_triangle_count = delta_vertex_count - 2;
    unsigned int delta_index_count = 3 * delta_triangle_count;

    polygon_copy_world_vertex(polygon, renderer->vertex_position_data + initial_vertex);

    Color color = polygon_get_color(polygon);
    for(int i = initial_vertex; i < initial_vertex + delta_vertex_count; i++)
        renderer->vertex_color_data[i] = color;

    renderer->vertex_count += delta_vertex_count;

    for(int i = 0; i < delta_triangle_count; i++)
    {
        renderer->index_data[initial_index + 3 * i] = initial_vertex;
        renderer->index_data[initial_index + 3 * i + 1] = initial_vertex + i + 1;
        renderer->index_data[initial_index + 3 * i + 2] = initial_vertex + i + 2;
    }

    renderer->index_count += delta_index_count;
}

void renderer_submit_vector(Renderer* renderer, Vector position, Vector direction, Color color)
{
    unsigned int initial_vertex = renderer->vector_vertex_count;
    unsigned int initial_index = renderer->vector_index_count;
    unsigned int delta_vertex_count = 4;
    unsigned int delta_index_count = 5;

    Vector initial_point = position;
    Vector terminal_point = vector_get_sum(position, direction);
    Vector u = vector_get_scaled(vector_get_normalized(vector_get_rotated(direction, 3.1416f * 0.9f)), 0.2f);
    Vector v = vector_get_scaled(vector_get_normalized(vector_get_rotated(direction, -3.1416f * 0.9f)), 0.2f);
    Vector left_tip_point = vector_get_sum(terminal_point, u);
    Vector right_tip_point = vector_get_sum(terminal_point, v);

    renderer->vector_vertex_position_data[initial_vertex] = initial_point;
    renderer->vector_vertex_position_data[initial_vertex + 1] = terminal_point;
    renderer->vector_vertex_position_data[initial_vertex + 2] = left_tip_point;
    renderer->vector_vertex_position_data[initial_vertex + 3] = right_tip_point;

    for(int i = initial_vertex; i < initial_vertex + delta_vertex_count; i++)
        renderer->vector_vertex_color_data[i] = color;

    renderer->vector_vertex_count += delta_vertex_count;

    renderer->vector_index_data[initial_index] = initial_vertex;
    renderer->vector_index_data[initial_index + 1] = initial_vertex + 1;
    renderer->vector_index_data[initial_index + 2] = initial_vertex + 2;
    renderer->vector_index_data[initial_index + 3] = initial_vertex + 3;
    renderer->vector_index_data[initial_index + 4] = initial_vertex + 1;

    renderer->vector_index_count += delta_index_count;
}

void renderer_flush(Renderer *renderer, Camera *camera)
{
    glClear(GL_COLOR_BUFFER_BIT);

    camera_copy_matrix(camera, renderer->matrix);
    glUniformMatrix4fv(renderer->matrix_location, 1, true, renderer->matrix);

    glBindVertexArray(renderer->vertex_array);

    glBindBuffer(GL_ARRAY_BUFFER, renderer->vertex_position_buffer);
    glBufferSubData(GL_ARRAY_BUFFER, 0, renderer->vertex_count * sizeof(Vector), renderer->vertex_position_data);

    glBindBuffer(GL_ARRAY_BUFFER, renderer->vertex_color_buffer);
    glBufferSubData(GL_ARRAY_BUFFER, 0, renderer->vertex_count * sizeof(Color), renderer->vertex_color_data);

    glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, renderer->index_count * sizeof(unsigned int), renderer->index_data);

    glDrawElements(GL_TRIANGLES, renderer->index_count, GL_UNSIGNED_INT, 0);

    renderer->vertex_count = 0;
    renderer->index_count = 0;

    glBindVertexArray(renderer->vector_vertex_array);

    glBindBuffer(GL_ARRAY_BUFFER, renderer->vector_vertex_position_buffer);
    glBufferSubData(GL_ARRAY_BUFFER, 0, renderer->vector_vertex_count * sizeof(Vector), renderer->vector_vertex_position_data);

    glBindBuffer(GL_ARRAY_BUFFER, renderer->vector_vertex_color_buffer);
    glBufferSubData(GL_ARRAY_BUFFER, 0, renderer->vector_vertex_count * sizeof(Color), renderer->vector_vertex_color_data);

    glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, renderer->vector_index_count * sizeof(unsigned int), renderer->vector_index_data);

    glDrawElements(GL_LINE_STRIP, renderer->vector_index_count, GL_UNSIGNED_INT, 0);

    renderer->vector_vertex_count = 0;
    renderer->vector_index_count = 0;
}
