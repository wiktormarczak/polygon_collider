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

#ifndef BATCH_H
#define BATCH_H

#include <polygon_collider/polygon_object.h>
#include <polygon_collider/geometry/vector.h>
#include <polygon_collider/geometry/edge.h>

typedef struct Batch Batch;

Batch *batch_create(unsigned int vertex_buffer_size, unsigned int index_buffer_size);
void batch_destroy(Batch *batch);

void batch_submit_polygon(Batch *batch, PolygonObject *polygon_object);
void batch_submit_edge(Batch *batch, Edge edge, Color color);
void batch_submit_triangle(Batch *batch, Vector *triangle, Color color);

void batch_use(Batch *batch);
void batch_draw_polygons(Batch *batch);
void batch_draw_edges(Batch *batch);
void batch_draw_triangles(Batch *batch);
void batch_flush(Batch *batch);

#endif
