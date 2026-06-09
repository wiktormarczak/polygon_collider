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

#define VERTEX_BUFFER_SIZE 100000
#define INDEX_BUFFER_SIZE 300000

#include <polygon_collider/renderer.h>
#include <polygon_collider/shader.h>
#include <polygon_collider/camera.h>
#include <polygon_collider/polygon.h>

#include <glad/gl.h>

#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

struct Renderer
{
    unsigned int shader_program;
    unsigned int matrix_location, color_location;

    unsigned int vertex_array;

    unsigned int vertex_buffer, index_buffer;
    unsigned int vertex_count, index_count;

    Vector vertex_data[VERTEX_BUFFER_SIZE];
    unsigned int index_data[INDEX_BUFFER_SIZE];

    float matrix[16];
};

Renderer *renderer_create()
{
    Renderer *renderer = malloc(sizeof(Renderer));

    renderer->shader_program = shader_create_program("glsl/vertex_shader.glsl", "glsl/fragment_shader.glsl");
    renderer->matrix_location = glGetUniformLocation(renderer->shader_program, "matrix");
    renderer->color_location = glGetUniformLocation(renderer->shader_program, "color");

    glGenVertexArrays(1, &renderer->vertex_array);
    glGenBuffers(1, &renderer->vertex_buffer);
    glGenBuffers(1, &renderer->index_buffer);

    glUseProgram(renderer->shader_program);
    glBindVertexArray(renderer->vertex_array);

    glBindBuffer(GL_ARRAY_BUFFER, renderer->vertex_buffer);
    glBufferData(GL_ARRAY_BUFFER, VERTEX_BUFFER_SIZE * sizeof(Vector), NULL, GL_DYNAMIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, renderer->index_buffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, INDEX_BUFFER_SIZE * sizeof(unsigned int), NULL, GL_DYNAMIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, false, 2 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);

    return renderer;
}

void renderer_destroy(Renderer *renderer)
{
    glDeleteProgram(renderer->shader_program);
    renderer->shader_program = 0;
    renderer->matrix_location = 0;
    renderer->color_location = 0;

    glDeleteVertexArrays(1, &renderer->vertex_array);
    renderer->vertex_array = 0;

    glDeleteBuffers(1, &renderer->vertex_buffer);
    renderer->vertex_buffer = 0;

    glDeleteBuffers(1, &renderer->index_buffer);
    renderer->index_buffer = 0;

    renderer->vertex_count = 0;
    renderer->index_count = 0;

    for(int i = 0; i < VERTEX_BUFFER_SIZE; i++)
        renderer->vertex_data[i].x = renderer->vertex_data[i].y = 0.0f;

    for(int i = 0; i < INDEX_BUFFER_SIZE; i++)
        renderer->index_data[i] = 0;

    free(renderer);
}

void renderer_submit(Renderer* renderer, Polygon *polygon)
{
    unsigned int initial_vertex = renderer->vertex_count;
    unsigned int initial_index = renderer->index_count;
    unsigned int delta_vertex_count = polygon_get_vertex_count(polygon);
    unsigned int delta_triangle_count = delta_vertex_count - 2;
    unsigned int delta_index_count = 3 * delta_triangle_count;

    polygon_copy_world_vertex(polygon, renderer->vertex_data + renderer->vertex_count);
    renderer->vertex_count += delta_vertex_count;

    for(int i = 0; i < delta_triangle_count; i++)
    {
        renderer->index_data[initial_index + 3 * i] = initial_vertex;
        renderer->index_data[initial_index + 3 * i + 1] = initial_vertex + i + 1;
        renderer->index_data[initial_index + 3 * i + 2] = initial_vertex + i + 2;
    }

    renderer->index_count += delta_index_count;
}

void renderer_flush(Renderer *renderer, Camera *camera)
{
    glClear(GL_COLOR_BUFFER_BIT);

    glBufferSubData(GL_ARRAY_BUFFER, 0, renderer->vertex_count * sizeof(Vector), renderer->vertex_data);
    glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, renderer->index_count * sizeof(unsigned int), renderer->index_data);

    camera_copy_matrix(camera, renderer->matrix);
    glUniformMatrix4fv(renderer->matrix_location, 1, true, renderer->matrix);

    glUniform3f(renderer->color_location, 1.0f, 1.0f, 1.0f);

    glDrawElements(GL_TRIANGLES, renderer->index_count, GL_UNSIGNED_INT, 0);

    renderer->vertex_count = 0;
    renderer->index_count = 0;
}
