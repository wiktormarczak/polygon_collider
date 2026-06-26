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

#include <polygon_collider/graphics/batch.h>
#include <glad/gl.h>
#include <stdlib.h>

struct Batch
{
    unsigned int vertex_array, buffer[3];

    unsigned int vertex_count, index_count;
    Vector *vertex_position_data;
    Color *vertex_color_data;
    unsigned int *index_data;
};

enum BufferType
{
    VERTEX_POSITION = 0,
    VERTEX_COLOR = 1,
    INDEX = 2
};

Batch *batch_create(unsigned int vertex_buffer_size, unsigned int index_buffer_size)
{
    Batch *batch = malloc(sizeof(Batch));

    glGenVertexArrays(1, &batch->vertex_array);
    glGenBuffers(3, batch->buffer);

    glBindVertexArray(batch->vertex_array);

    glBindBuffer(GL_ARRAY_BUFFER, batch->buffer[VERTEX_POSITION]);
    glBufferData(GL_ARRAY_BUFFER, vertex_buffer_size * sizeof(Vector), NULL, GL_DYNAMIC_DRAW);
    glVertexAttribPointer(0, 2, GL_DOUBLE, false, sizeof(Vector), (void *)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, batch->buffer[VERTEX_COLOR]);
    glBufferData(GL_ARRAY_BUFFER, vertex_buffer_size * sizeof(Color), NULL, GL_DYNAMIC_DRAW);
    glVertexAttribPointer(1, 3, GL_FLOAT, false, sizeof(Color), (void *)0);
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, batch->buffer[INDEX]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_buffer_size * sizeof(unsigned int), NULL, GL_DYNAMIC_DRAW);

    batch->vertex_count = 0;
    batch->index_count = 0;

    batch->vertex_position_data = malloc(vertex_buffer_size * sizeof(Vector));
    batch->vertex_color_data = malloc(vertex_buffer_size * sizeof(Color));
    batch->index_data = malloc(index_buffer_size * sizeof(unsigned int));

    return batch;
}

void batch_destroy(Batch *batch)
{
    glDeleteVertexArrays(1, &batch->vertex_array);
    batch->vertex_array = 0;

    glDeleteBuffers(3, batch->buffer);
    for(int i = 0; i < 3; i++)
        batch->buffer[i] = 0;

    for(int i = 0; i < batch->vertex_count; i++)
    {
        batch->vertex_position_data[i].x = 0.0f;
        batch->vertex_position_data[i].y = 0.0f;
        batch->vertex_color_data[i].r = 0.0f;
        batch->vertex_color_data[i].g = 0.0f;
        batch->vertex_color_data[i].b = 0.0f;
    }

    for(int i = 0; i < batch->index_count; i++)
        batch->index_data[i] = 0;

    batch->vertex_count = 0;
    batch->index_count = 0;

    free(batch->vertex_position_data);
    batch->vertex_position_data = NULL;

    free(batch->vertex_color_data);
    batch->vertex_color_data = NULL;

    free(batch->index_data);
    batch->index_data = NULL;

    free(batch);
    batch = NULL;
}

void batch_submit_polygon(Batch *batch, PolygonObject *polygon)
{
    unsigned int initial_vertex = batch->vertex_count;
    unsigned int initial_index = batch->index_count;
    unsigned int delta_vertex_count = polygon_object_get_vertex_count(polygon);
    unsigned int delta_triangle_count = delta_vertex_count - 2;
    unsigned int delta_index_count = 3 * delta_triangle_count;

    polygon_object_copy_world_vertex(polygon, batch->vertex_position_data + initial_vertex);

    Color color = polygon_object_get_color(polygon);
    for(int i = initial_vertex; i < initial_vertex + delta_vertex_count; i++)
        batch->vertex_color_data[i] = color;

    batch->vertex_count += delta_vertex_count;

    for(int i = 0; i < delta_triangle_count; i++)
    {
        batch->index_data[initial_index + 3 * i] = initial_vertex;
        batch->index_data[initial_index + 3 * i + 1] = initial_vertex + i + 1;
        batch->index_data[initial_index + 3 * i + 2] = initial_vertex + i + 2;
    }

    batch->index_count += delta_index_count;
}

void batch_submit_edge(Batch *batch, Edge edge, Color color)
{
    unsigned int initial_vertex = batch->vertex_count;
    batch->vertex_position_data[initial_vertex] = edge.tail;
    batch->vertex_position_data[initial_vertex + 1] = edge.head;
    batch->vertex_color_data[initial_vertex] = color;
    batch->vertex_color_data[initial_vertex + 1] = color;
    batch->vertex_count += 2;
}

void batch_submit_triangle(Batch *batch, Vector triangle[3], Color color)
{
    unsigned int initial_vertex = batch->vertex_count;
    batch->vertex_position_data[initial_vertex] = triangle[0];
    batch->vertex_position_data[initial_vertex + 1] = triangle[1];
    batch->vertex_position_data[initial_vertex + 2] = triangle[2];
    batch->vertex_color_data[initial_vertex] = color;
    batch->vertex_color_data[initial_vertex + 1] = color;
    batch->vertex_color_data[initial_vertex + 2] = color;
    batch->vertex_count += 3;
}

void batch_use(Batch *batch)
{
    glBindVertexArray(batch->vertex_array);

    glBindBuffer(GL_ARRAY_BUFFER, batch->buffer[VERTEX_POSITION]);
    glBufferSubData(GL_ARRAY_BUFFER, 0, batch->vertex_count * sizeof(Vector), batch->vertex_position_data);

    glBindBuffer(GL_ARRAY_BUFFER, batch->buffer[VERTEX_COLOR]);
    glBufferSubData(GL_ARRAY_BUFFER, 0, batch->vertex_count * sizeof(Color), batch->vertex_color_data);

    glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, batch->index_count * sizeof(unsigned int), batch->index_data);
}

void batch_draw_polygons(Batch *batch)
{
    glDrawElements(GL_TRIANGLES, batch->index_count, GL_UNSIGNED_INT, 0);
}

void batch_draw_edges(Batch *batch)
{
    glDrawArrays(GL_LINES, 0, batch->vertex_count);
}

void batch_draw_triangles(Batch *batch)
{
    glDrawArrays(GL_TRIANGLES, 0, batch->vertex_count);
}

void batch_flush(Batch *batch)
{
    batch->vertex_count = 0;
    batch->index_count = 0;
}
