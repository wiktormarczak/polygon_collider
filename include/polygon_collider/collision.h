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

#ifndef COLLISION_H
#define COLLISION_H

#include <polygon_collider/polygon.h>
#include <polygon_collider/vector_object_queue.h>

bool collision_handle(Polygon *left, Polygon *right, VectorObjectQueue *vector_object_queue);
void collision_handle_with_wall(Polygon *polygon, VectorObjectQueue *vector_object_queue);
bool collision_check(Polygon *left, Polygon *right, Vector *contact_point_destination, Vector *axis_destination);
bool collision_is_point_inside(Vector point, unsigned int vertex_count, Vector *vertex);

#endif
