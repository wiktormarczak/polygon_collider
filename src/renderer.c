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

#include <polygon_collider/renderer.h>
#include <polygon_collider/shader.h>
#include <polygon_collider/camera.h>
#include <polygon_collider/polygon_object.h>
#include <polygon_collider/geometry/vector.h>
#include <polygon_collider/color.h>
#include <polygon_collider/batch.h>
#include <polygon_collider/geometry/edge.h>

#include <glad/gl.h>

#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>

struct Renderer
{
    unsigned int shader_program;
    unsigned int matrix_location;

    Batch *polygon_batch, *vector_edge_batch, *vector_tip_batch;

    float matrix[16];
};

Renderer *renderer_create()
{
    Renderer *renderer = malloc(sizeof(Renderer));

    renderer->shader_program = shader_create_program("glsl/vertex_shader.glsl", "glsl/fragment_shader.glsl");
    renderer->matrix_location = glGetUniformLocation(renderer->shader_program, "matrix");

    glUseProgram(renderer->shader_program);
    renderer->polygon_batch = batch_create(100, 300);
    renderer->vector_edge_batch = batch_create(300, 0);
    renderer->vector_tip_batch = batch_create(200, 0);

    return renderer;
}

void renderer_destroy(Renderer *renderer)
{
    glDeleteProgram(renderer->shader_program);
    renderer->shader_program = 0;
    renderer->matrix_location = 0;

    batch_destroy(renderer->polygon_batch);
    renderer->polygon_batch = NULL;

    batch_destroy(renderer->vector_edge_batch);
    renderer->vector_edge_batch = NULL;

    batch_destroy(renderer->vector_tip_batch);
    renderer->vector_tip_batch = NULL;

    free(renderer);
}

void renderer_submit_polygon(Renderer* renderer, PolygonObject *polygon_object)
{
    batch_submit_polygon(renderer->polygon_batch, polygon_object);
}

void renderer_submit_vector(Renderer* renderer, VectorObject *vector)
{
    const float tip_width = 0.1f, tip_length = 0.2f;

    Color color = vector_object_get_color(vector);

    Edge edge = vector_object_get_edge(vector);

    Vector tip[3];
    vector_object_copy_tip(vector, tip);

    batch_submit_edge(renderer->vector_edge_batch, edge, color);
    batch_submit_triangle(renderer->vector_tip_batch, tip, color);
}

void renderer_submit_vector_queue(Renderer *renderer, VectorObjectQueue *vector_object_queue)
{
    unsigned int count = vector_object_queue_get_count(vector_object_queue);
    for(unsigned int i = 0; i < count; i++)
    {
        VectorObject *vector_object = vector_object_queue_get_vector_object(vector_object_queue, i);
        renderer_submit_vector(renderer, vector_object);
    }
}

void renderer_flush(Renderer *renderer, Camera *camera)
{
    glClear(GL_COLOR_BUFFER_BIT);

    camera_copy_matrix(camera, renderer->matrix);
    glUniformMatrix4fv(renderer->matrix_location, 1, true, renderer->matrix);

    batch_use(renderer->polygon_batch);
    batch_draw_polygons(renderer->polygon_batch);
    batch_flush(renderer->polygon_batch);

    batch_use(renderer->vector_edge_batch);
    batch_draw_edges(renderer->vector_edge_batch);
    batch_flush(renderer->vector_edge_batch);

    batch_use(renderer->vector_tip_batch);
    batch_draw_triangles(renderer->vector_tip_batch);
    batch_flush(renderer->vector_tip_batch);
}
