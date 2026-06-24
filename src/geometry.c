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

#include <polygon_collider/geometry.h>
#include <polygon_collider/collision.h>
#include <polygon_collider/geometry/line.h>
#include <float.h>
#include <math.h>

static float geometry_get_segment_angular_mass(Line line, float left, float right);

float geometry_get_linear_mass(unsigned int vertex_count, Vector *vertex)
{
    float linear_mass = 0.0f;
    for(int i = 0; i < vertex_count; i++)
        linear_mass += vector_get_cross_product(vertex[i], vertex[(i + 1) % vertex_count]);

    linear_mass = fabs(linear_mass);
    linear_mass /= 2.0f;

    return linear_mass;
}

float geometry_get_linear_mass_brutally(unsigned int vertex_count, Vector *vertex)
{
    float min_x = FLT_MAX, min_y = FLT_MAX, max_x = FLT_MIN, max_y = FLT_MIN;
    for(int i = 0; i < vertex_count; i++)
    {
        if(vertex[i].x < min_x)
            min_x = vertex[i].x;

        if(vertex[i].x > max_x)
            max_x = vertex[i].x;

        if(vertex[i].y < min_y)
            min_y = vertex[i].y;

        if(vertex[i].y > max_y)
            max_y = vertex[i].y;
    }

    const float depth = 0.1f;
    float linear_mass = 0.0f;
    for(float x = min_x; x < max_x; x += depth)
        for(float y = min_y; y < max_y; y += depth)
            if(collision_is_point_inside(vector_get(x, y), vertex_count, vertex))
                linear_mass += depth * depth;

    return linear_mass;
}

float geometry_get_angular_mass(unsigned int vertex_count, Vector *vertex)
{
    float angular_mass = 0.0f;
    for(int i = 0; i < vertex_count; i++)
    {
        Vector u, v;
        u.x = vector_get_length(vertex[i]);
        u.y = 0.0f;
        v.x = vector_get_dot_product(vertex[i], vertex[(i + 1) % vertex_count]) / u.x;
        v.y = vector_get_cross_product(vertex[i], vertex[(i + 1) % vertex_count]) / u.x;

        Line line_1 = line_get_from_points(vector_get(0.0f, 0.0f), v);
        Line line_2 = line_get_from_points(v, u);

        float delta_1 = geometry_get_segment_angular_mass(line_1, 0.0f, v.x);
        float delta_2 = geometry_get_segment_angular_mass(line_2, v.x, u.x);

        float delta = delta_1 + delta_2;
        angular_mass += delta;
    }

    return angular_mass;
}

float geometry_get_angular_mass_brutally(unsigned int vertex_count, Vector *vertex)
{
    float min_x = FLT_MAX, min_y = FLT_MAX, max_x = FLT_MIN, max_y = FLT_MIN;
    for(int i = 0; i < vertex_count; i++)
    {
        if(vertex[i].x < min_x)
            min_x = vertex[i].x;

        if(vertex[i].x > max_x)
            max_x = vertex[i].x;

        if(vertex[i].y < min_y)
            min_y = vertex[i].y;

        if(vertex[i].y > max_y)
            max_y = vertex[i].y;
    }

    const float depth = 0.1f;
    float angular_mass = 0.0f;
    for(float x = min_x; x < max_x; x += depth)
        for(float y = min_y; y < max_y; y += depth)
            if(collision_is_point_inside(vector_get(x, y), vertex_count, vertex))
                angular_mass += depth * depth * (x * x + y * y);

    return angular_mass;
}

static float geometry_get_segment_angular_mass(Line line, float left, float right)
{
    float parameter[5];
    parameter[0] = 0.0f;
    parameter[1] = line.intercept * line.intercept * line.intercept / 3.0f;
    parameter[2] = line.slope * line.intercept * line.intercept;
    parameter[3] = line.slope * line.slope * line.intercept + line.intercept;
    parameter[4] = line.slope * line.slope * line.slope / 3.0f + line.slope;

    float left_power = 1.0f;
    float right_power = 1.0f;
    float angular_mass = 0.0f;
    for(int i = 1; i <= 4; i++)
    {
        left_power *= left;
        right_power *= right;

        float delta = parameter[i] * (right_power - left_power) / (float)i;
        angular_mass += delta;
    }

    return angular_mass;
}
