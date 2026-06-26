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

void polygon_copy(Polygon *source, Polygon *destination)
{
    if(source->vertex_count != destination->vertex_count)
    {
        destination->vertex_count = source->vertex_count;
        destination->vertex = realloc(destination->vertex, destination->vertex_count * sizeof(Vector));
    }

    memcpy(destination->vertex, source->vertex, destination->vertex_count * sizeof(Vector));
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
            if(polygon_collision_is_point_inside(vector_get(x, y), polygon->vertex_count, polygon->vertex))
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
            if(polygon_collision_is_point_inside(vector_get(x, y), polygon->vertex_count, polygon->vertex))
                angular_mass += depth * depth * (x * x + y * y);

    return angular_mass;
}

void polygon_copy_vertex(Polygon *polygon, Vector *vertex_destination)
{
    memcpy(vertex_destination, polygon->vertex, polygon->vertex_count * sizeof(Vector));
}

void polygon_translate(Polygon *polygon, Vector translation)
{
    for(unsigned int i = 0; i < polygon->vertex_count; i++)
        vector_add(&polygon->vertex[i], translation);
}

void polygon_rotate(Polygon *polygon, double rotation)
{
    for(unsigned int i = 0; i < polygon->vertex_count; i++)
        vector_rotate(&polygon->vertex[i], rotation);
}

// Collisions

bool polygon_collision_check(Polygon *left, Polygon *right, Vector *contact_point_destination, Vector *axis_destination)
{
    Vector contact_point_left, axis_left, contact_point_right, axis_right;
    double overlap_left = polygon_collision_get_min_overlap(left, right, &contact_point_left, &axis_left);
    double overlap_right = polygon_collision_get_min_overlap(right, left, &contact_point_right, &axis_right);

    double overlap = overlap_left;
    Vector contact_point = contact_point_left;
    Vector axis = vector_get_scaled(axis_left, overlap_left);

    if(overlap_right < overlap)
    {
        overlap = overlap_right;
        contact_point = contact_point_right;
        axis = vector_get_scaled(axis_right, -overlap_right);
    }

    if(overlap <= 0.0)
        return false;

    *contact_point_destination = contact_point;
    *axis_destination = axis;
    return true;
}

bool polygon_collision_is_point_inside(Vector point, unsigned int vertex_count, Vector *vertex)
{
    for(int i = 0; i < vertex_count; i++)
    {
        Edge edge = edge_get(vertex[i], vertex[(i + 1) % vertex_count]);
        Vector axis = edge_get_normal(edge);
        double point_value = vector_get_dot_product(axis, point);
        double polygon_value = vector_get_dot_product(axis, vertex[i]);

        if(point_value > polygon_value)
            return false;
    }

    return true;
}

double polygon_collision_get_min_overlap(Polygon *left, Polygon *right, Vector *contact_point_destination, Vector *axis_destination)
{
    double min_overlap = FLT_MAX;
    Vector min_contact_point, min_axis;

    for(int i = 0; i < left->vertex_count; i++)
    {
        Edge edge = edge_get(left->vertex[i], left->vertex[(i + 1) % left->vertex_count]);
        Vector axis = edge_get_normal(edge);

        Vector contact_point;
        double left_max = vector_get_dot_product(axis, left->vertex[i]);
        double right_min = polygon_collision_get_projection_min(axis, edge_get(left->vertex[i], left->vertex[(i + 1) % left->vertex_count]), right, &contact_point);

        double overlap = left_max - right_min;

        if(overlap < min_overlap)
        {
            min_overlap = overlap;
            min_contact_point = contact_point;
            min_axis = axis;
        }
    }

    *contact_point_destination = min_contact_point;
    *axis_destination = vector_get_negative(min_axis);
    return min_overlap;
}

double polygon_collision_get_projection_min(Vector axis, Edge edge, Polygon *collision_box, Vector *contact_point_destination)
{
    double min = FLT_MAX;
    Vector contact_point_1, contact_point_2;
    bool db = false;

    for(int i = 0; i < collision_box->vertex_count; i++)
    {
        double value = vector_get_dot_product(axis, collision_box->vertex[i]);

        if(fabs(value - min) < 0.01f)
        {
            contact_point_2 = collision_box->vertex[i];
            db = true;
            continue;
        }

        if(value < min)
        {
            min = value;
            contact_point_1 = collision_box->vertex[i];
            db = false;
            continue;
        }
    }

    Vector contact_point = contact_point_1;

    if(db)
    {
        Vector point[4];
        point[0] = edge.initial_point;
        point[1] = edge.terminal_point;
        point[2] = contact_point_1;
        point[3] = contact_point_2;

        for(int i = 0; i < 4; i++)
        {
            double min_v = FLT_MAX;
            unsigned int min_j = -1;

            for(int j = i; j < 4; j++)
            {
                double v = vector_get_cross_product(axis, point[j]);

                if(v < min_v)
                {
                    min_v = v;
                    min_j = j;
                }
            }

            Vector buffer = point[i];
            point[i] = point[min_j];
            point[min_j] = buffer;
        }

        contact_point = vector_get_average(point[1], point[2]);
    }

    *contact_point_destination = contact_point;
    return min;
}
