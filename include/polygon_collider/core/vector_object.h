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

#ifndef VECTOR_OBJECT_H
#define VECTOR_OBJECT_H

#include <polygon_collider/geometry/vector.h>
#include <polygon_collider/graphics/color.h>
#include <polygon_collider/geometry/edge.h>

typedef struct VectorObject VectorObject;

VectorObject *vector_object_create();
void vector_object_destroy(VectorObject *vector_object);

void vector_object_set_vector(VectorObject *vector_object, Vector vector);
void vector_object_set_position(VectorObject *vector_object, Vector position);
void vector_object_set_color(VectorObject *vector_object, Color color);

Edge vector_object_get_edge(VectorObject *vector_object);
Color vector_object_get_color(VectorObject *vector_object);

void vector_object_copy_tip(VectorObject *vector_object, Vector *tip_destination);

#endif
