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

#include <polygon_collider/geometry/polygon.h>
#include <polygon_collider/geometry/vector.h>
#include <polygon_collider/geometry/line.h>
#include <polygon_collider/collision.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <float.h>

struct Polygon
{
    unsigned int vertex_count;
    Vector *vertex;
};

Polygon *polygon_create_regular(unsigned int vertex_count, float radius)
{
    const double pi = 3.14159;

    if(vertex_count < 3)
        return NULL;

    Polygon *polygon = malloc(sizeof(Polygon));

    polygon->vertex_count = vertex_count;
    polygon->vertex = malloc(vertex_count * sizeof(Vector));

    double angle = 2.0 * pi / vertex_count;
    for(unsigned int i = 0; i < vertex_count; i++)
    {
        polygon->vertex[i].x = cos(i * angle) * radius;
        polygon->vertex[i].y = sin(i * angle) * radius;
    }

    return polygon;
}

void polygon_destroy(Polygon *polygon)
{
    for(unsigned int i = 0; i < polygon->vertex_count; i++)
    {
        polygon->vertex[i].x = 0.0;
        polygon->vertex[i].y = 0.0;
    }

    free(polygon->vertex);
    polygon->vertex = NULL;

    polygon->vertex_count = 0;

    free(polygon);
    polygon = NULL;
}

unsigned int polygon_get_vertex_count(Polygon *polygon)
{
    return polygon->vertex_count;
}

double polygon_get_linear_mass(Polygon *polygon)
{
    float linear_mass = 0.0f;
    for(int i = 0; i < polygon->vertex_count; i++)
        linear_mass += vector_get_cross_product(polygon->vertex[i], polygon->vertex[(i + 1) % polygon->vertex_count]);

    linear_mass = fabs(linear_mass);
    linear_mass /= 2.0f;

    return linear_mass;
}

double polygon_get_linear_mass_brutally(Polygon *polygon)
{
    float min_x = FLT_MAX, min_y = FLT_MAX, max_x = FLT_MIN, max_y = FLT_MIN;
    for(int i = 0; i < polygon->vertex_count; i++)
    {
        if(polygon->vertex[i].x < min_x)
            min_x = polygon->vertex[i].x;

        if(polygon->vertex[i].x > max_x)
            max_x = polygon->vertex[i].x;

        if(polygon->vertex[i].y < min_y)
            min_y = polygon->vertex[i].y;

        if(polygon->vertex[i].y > max_y)
            max_y = polygon->vertex[i].y;
    }

    const float depth = 0.1f;
    float linear_mass = 0.0f;
    for(float x = min_x; x < max_x; x += depth)
        for(float y = min_y; y < max_y; y += depth)
            if(collision_is_point_inside(vector_get(x, y), polygon->vertex_count, polygon->vertex))
                linear_mass += depth * depth;

    return linear_mass;
}

double polygon_get_angular_mass(Polygon *polygon)
{
    float angular_mass = 0.0f;
    for(int i = 0; i < polygon->vertex_count; i++)
    {
        Vector u, v;
        u.x = vector_get_length(polygon->vertex[i]);
        u.y = 0.0f;
        v.x = vector_get_dot_product(polygon->vertex[i], polygon->vertex[(i + 1) % polygon->vertex_count]) / u.x;
        v.y = vector_get_cross_product(polygon->vertex[i], polygon->vertex[(i + 1) % polygon->vertex_count]) / u.x;

        Line line_1 = line_get_from_points(vector_get(0.0f, 0.0f), v);
        Line line_2 = line_get_from_points(v, u);

        float delta_1 = line_get_segment_angular_mass(line_1, 0.0f, v.x);
        float delta_2 = line_get_segment_angular_mass(line_2, v.x, u.x);

        float delta = delta_1 + delta_2;
        angular_mass += delta;
    }

    return angular_mass;
}

double polygon_get_angular_mass_brutally(Polygon *polygon)
{
    float min_x = FLT_MAX, min_y = FLT_MAX, max_x = FLT_MIN, max_y = FLT_MIN;
    for(int i = 0; i < polygon->vertex_count; i++)
    {
        if(polygon->vertex[i].x < min_x)
            min_x = polygon->vertex[i].x;

        if(polygon->vertex[i].x > max_x)
            max_x = polygon->vertex[i].x;

        if(polygon->vertex[i].y < min_y)
            min_y = polygon->vertex[i].y;

        if(polygon->vertex[i].y > max_y)
            max_y = polygon->vertex[i].y;
    }

    const float depth = 0.1f;
    float angular_mass = 0.0f;
    for(float x = min_x; x < max_x; x += depth)
        for(float y = min_y; y < max_y; y += depth)
            if(collision_is_point_inside(vector_get(x, y), polygon->vertex_count, polygon->vertex))
                angular_mass += depth * depth * (x * x + y * y);

    return angular_mass;
}

void polygon_copy_vertex(Polygon *polygon, Vector *vertex_destination)
{
    memcpy(vertex_destination, polygon->vertex, polygon->vertex_count * sizeof(Vector));
}
