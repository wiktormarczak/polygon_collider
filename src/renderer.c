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
#include <polygon_collider/polygon.h>
#include <polygon_collider/vector.h>
#include <polygon_collider/color.h>
#include <polygon_collider/batch.h>
#include <polygon_collider/edge.h>

#include <glad/gl.h>

#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

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

void renderer_submit_polygon(Renderer* renderer, Polygon *polygon)
{
    batch_submit_polygon(renderer->polygon_batch, polygon);
}

void renderer_submit_vector(Renderer* renderer, Vector position, Vector direction, Color color)
{
    const float tip_width = 0.1f, tip_length = 0.2f;

    Edge edge;
    edge.initial_point = position;
    edge.terminal_point = vector_get_sum(position, direction);

    Vector tip[3];
    tip[0] = edge.terminal_point;
    tip[1] = vector_get_sum(edge.terminal_point, vector_get_scaled(vector_get_normalized(vector_get_rotated(direction, 3.14f * (1.0f - tip_width))), tip_length));
    tip[2] = vector_get_sum(edge.terminal_point, vector_get_scaled(vector_get_normalized(vector_get_rotated(direction, -3.14f * (1.0f - tip_width))), tip_length));

    batch_submit_edge(renderer->vector_edge_batch, edge, color);
    batch_submit_triangle(renderer->vector_tip_batch, tip, color);
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
