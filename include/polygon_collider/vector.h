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

#ifndef VECTOR_H
#define VECTOR_H

typedef struct
{
    float x, y;
} Vector;

Vector vector_get(float x, float y);

float vector_get_length(Vector vector);

Vector vector_get_normalized(Vector vector);
Vector vector_get_perpendicular(Vector vector);

Vector vector_get_scaled(Vector vector, float scalar);
Vector vector_get_rotated(Vector vector, float angle);

Vector vector_get_sum(Vector left, Vector right);
Vector vector_get_difference(Vector left, Vector right);
float vector_get_dot_product(Vector left, Vector right);

Vector vector_get_normal(Vector left, Vector right);

#endif
