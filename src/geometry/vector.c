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

#include <polygon_collider/geometry/vector.h>
#include <math.h>

Vector vector_get(double x, double y)
{
    Vector vector;
    vector.x = x;
    vector.y = y;
    return vector;
}

Vector vector_get_normalized(Vector vector)
{
    return vector_get_resized(vector, 1.0f);
}

Vector vector_get_negative(Vector vector)
{
    return vector_get_scaled(vector, -1.0);
}

Vector vector_get_orthogonal(Vector vector)
{
    const double pi = 3.141592653589793;
    return vector_get_rotated(vector, -pi / 2.0);
}

Vector vector_get_scaled(Vector vector, double scalar)
{
    vector.x *= scalar;
    vector.y *= scalar;
    return vector;
}

Vector vector_get_resized(Vector vector, double size)
{
    double length = vector_get_length(vector);
    return vector_get_scaled(vector, size / length);
}

Vector vector_get_rotated(Vector vector, double angle)
{
    double sine = sin(angle);
    double cosine = cos(angle);

    Vector rotated;
    rotated.x = vector.x * cosine - vector.y * sine;
    rotated.y = vector.x * sine + vector.y * cosine;
    return rotated;
}

double vector_get_length(Vector vector)
{
    return sqrt(vector_get_dot_product(vector, vector));
}

void vector_normalize(Vector *vector)
{
    *vector = vector_get_normalized(*vector);
}

void vector_negate(Vector *vector)
{
    *vector = vector_get_negative(*vector);
}

void vector_orthogonalize(Vector *vector)
{
    *vector = vector_get_orthogonal(*vector);
}

void vector_scale(Vector *vector, double scalar)
{
    *vector = vector_get_scaled(*vector, scalar);
}

void vector_resize(Vector *vector, double size)
{
    *vector = vector_get_resized(*vector, size);
}

void vector_rotate(Vector *vector, double angle)
{
    *vector = vector_get_rotated(*vector, angle);
}

void vector_add(Vector *vector, Vector summand)
{
    *vector = vector_get_sum(*vector, summand);
}

void vector_subtract(Vector *vector, Vector subtrahend)
{
    *vector = vector_get_difference(*vector, subtrahend);
}

Vector vector_get_sum(Vector left, Vector right)
{
    Vector vector;
    vector.x = left.x + right.x;
    vector.y = left.y + right.y;
    return vector;
}

Vector vector_get_difference(Vector left, Vector right)
{
    Vector vector;
    vector.x = left.x - right.x;
    vector.y = left.y - right.y;
    return vector;
}

Vector vector_get_average(Vector left, Vector right)
{
    return vector_get_scaled(vector_get_sum(left, right), 0.5f);
}

double vector_get_dot_product(Vector left, Vector right)
{
    return left.x * right.x + left.y * right.y;
}

double vector_get_cross_product(Vector left, Vector right)
{
    return left.x * right.y - left.y * right.x;
}
