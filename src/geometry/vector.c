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

Vector vector_get(float x, float y)
{
    Vector vector;
    vector.x = x;
    vector.y = y;
    return vector;
}

float vector_get_length(Vector vector)
{
    return sqrt(vector_get_dot_product(vector, vector));
}

Vector vector_get_normalized(Vector vector)
{
    float length = vector_get_length(vector);
    vector = vector_get_scaled(vector, 1.0f / length);
    return vector;
}

Vector vector_get_perpendicular(Vector vector)
{
    Vector perpendicular;
    perpendicular.x = vector.y;
    perpendicular.y = -vector.x;
    return perpendicular;
}

Vector vector_get_negated(Vector vector)
{
    vector.x = -vector.x;
    vector.y = -vector.y;
    return vector;
}

Vector vector_get_scaled(Vector vector, float scalar)
{
    vector.x *= scalar;
    vector.y *= scalar;
    return vector;
}

Vector vector_get_resized(Vector vector, float size)
{
    return vector_get_scaled(vector_get_normalized(vector), size);
}

Vector vector_get_rotated(Vector vector, float angle)
{
    Vector rotated;
    rotated.x = vector.x * cos(angle) - vector.y * sin(angle);
    rotated.y = vector.x * sin(angle) + vector.y * cos(angle);
    return rotated;
}

Vector vector_get_sum(Vector left, Vector right)
{
    left.x += right.x;
    left.y += right.y;
    return left;
}

Vector vector_get_difference(Vector left, Vector right)
{
    left.x -= right.x;
    left.y -= right.y;
    return left;
}

Vector vector_get_average(Vector left, Vector right)
{
    return vector_get_scaled(vector_get_sum(left, right), 0.5f);
}

float vector_get_dot_product(Vector left, Vector right)
{
    return left.x * right.x + left.y * right.y;
}

float vector_get_cross_product(Vector left, Vector right)
{
    return left.x * right.y - left.y * right.x;
}

Vector vector_get_normal(Vector left, Vector right)
{
    Vector edge = vector_get_difference(right, left);
    Vector normal = vector_get_perpendicular(edge);
    return vector_get_normalized(normal);
}
