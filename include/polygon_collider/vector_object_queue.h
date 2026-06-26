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

#ifndef VECTOR_OBJECT_QUEUE_H
#define VECTOR_OBJECT_QUEUE_H

#include <polygon_collider/vector_object.h>

typedef struct VectorObjectQueue VectorObjectQueue;

VectorObjectQueue *vector_object_queue_create();
void vector_object_queue_destroy(VectorObjectQueue *vector_object_queue);

void vector_object_queue_submit_vector(VectorObjectQueue *vector_object_queue, VectorObject *vector_object);
void vector_object_queue_update(VectorObjectQueue *vector_object_queue, float delta_time);

unsigned int vector_object_queue_get_count(VectorObjectQueue *vector_object_queue);
VectorObject *vector_object_queue_get_vector_object(VectorObjectQueue *vector_object_queue, unsigned int i);

#endif
