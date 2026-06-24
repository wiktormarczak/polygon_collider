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
    double x, y;
} Vector;

Vector vector_get(double x, double y);

Vector vector_get_normalized(Vector vector);
Vector vector_get_negative(Vector vector);
Vector vector_get_orthogonal(Vector vector);

Vector vector_get_scaled(Vector vector, double scalar);
Vector vector_get_resized(Vector vector, double size);
Vector vector_get_rotated(Vector vector, double angle);

double vector_get_length(Vector vector);

void vector_normalize(Vector *vector);
void vector_negate(Vector *vector);
void vector_orthogonalize(Vector *vector);

void vector_scale(Vector *vector, double scalar);
void vector_resize(Vector *vector, double size);
void vector_rotate(Vector *vector, double angle);

void vector_add(Vector *vector, Vector summand);
void vector_subtract(Vector *vector, Vector subtrahend);

Vector vector_get_sum(Vector left, Vector right);
Vector vector_get_difference(Vector left, Vector right);

Vector vector_get_average(Vector left, Vector right);

double vector_get_dot_product(Vector left, Vector right);
double vector_get_cross_product(Vector left, Vector right);

// To Be Deleted
Vector vector_get_normal(Vector left, Vector right);

#endif
