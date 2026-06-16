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

#define PI 3.1416f

#include <polygon_collider/polygon.h>
#include <polygon_collider/vector.h>
#include <polygon_collider/color.h>
#include <polygon_collider/geometry.h>

#include <math.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

struct Polygon
{
    unsigned int vertex_count;
    Vector *vertex, *world_vertex;

    Vector position;
    float orientation;

    float linear_mass;
    float angular_mass;

    Vector linear_velocity;
    float angular_velocity;

    Color color;
};

Polygon *polygon_create(unsigned int vertex_count, Vector *vertex, Color color)
{
    Polygon *polygon = malloc(sizeof(Polygon));

    polygon->vertex_count = vertex_count;

    polygon->vertex = malloc(vertex_count * sizeof(Vector));
    memcpy(polygon->vertex, vertex, vertex_count * sizeof(Vector));

    polygon->world_vertex = malloc(vertex_count * sizeof(Vector));
    memcpy(polygon->world_vertex, vertex, vertex_count * sizeof(Vector));

    polygon->position.x = 0.0f;
    polygon->position.y = 0.0f;
    polygon->orientation = 0.0f;

    polygon->linear_mass = geometry_get_linear_mass(vertex_count, vertex);
    polygon->angular_mass = geometry_get_angular_mass(vertex_count, vertex);

    polygon->linear_velocity.x = 0.0f;
    polygon->linear_velocity.y = 0.0f;
    polygon->angular_velocity = 0.0f;

    polygon->color = color;

    return polygon;
}

Polygon *polygon_create_regular(unsigned int vertex_count, float radius, Color color)
{
    if(vertex_count < 3)
        return NULL;

    Vector *vertex = malloc(vertex_count * sizeof(Vector));

    float delta_angle = (2.0f * PI) / vertex_count;
    for(int i = 0; i < vertex_count; i++)
    {
        vertex[i].x = radius * cos(i * delta_angle);
        vertex[i].y = radius * sin(i * delta_angle);
    }

    Polygon *polygon = polygon_create(vertex_count, vertex, color);
    free(vertex);
    return polygon;
}

void polygon_destroy(Polygon *polygon)
{
    free(polygon->vertex);
    polygon->vertex = NULL;

    free(polygon);
}

unsigned int polygon_get_vertex_count(Polygon *polygon)
{
    return polygon->vertex_count;
}

Color polygon_get_color(Polygon *polygon)
{
    return polygon->color;
}

void polygon_copy_world_vertex(Polygon *polygon, Vector *destination)
{
    memcpy(destination, polygon->world_vertex, polygon->vertex_count * sizeof(Vector));
}

void polygon_set_position(Polygon *polygon, Vector position)
{
    polygon->position = position;
}

void polygon_set_orientation(Polygon *polygon, float orientation)
{
    polygon->orientation = orientation;
}

void polygon_adjust_linear_velocity(Polygon *polygon, Vector delta)
{
    polygon->linear_velocity.x += delta.x;
    polygon->linear_velocity.y += delta.y;
}

void polygon_adjust_angular_velocity(Polygon *polygon, float delta)
{
    polygon->angular_velocity += delta;
}

void polygon_set_color(Polygon *polygon, Color color)
{
    polygon->color = color;
}

void polygon_update(Polygon *polygon, float delta_time)
{
    polygon->position.x += delta_time * polygon->linear_velocity.x;
    polygon->position.y += delta_time * polygon->linear_velocity.y;
    polygon->orientation += delta_time * polygon->angular_velocity;

    polygon->orientation = fmod(polygon->orientation, 2.0f * PI);

    for(int i = 0; i < polygon->vertex_count; i++)
    {
        float x = polygon->vertex[i].x;
        float y = polygon->vertex[i].y;
        float s = sin(polygon->orientation);
        float c = cos(polygon->orientation);
        polygon->world_vertex[i].x = x * c - y * s + polygon->position.x;
        polygon->world_vertex[i].y = x * s + y * c + polygon->position.y;
    }
}

void polygon_apply_impulse(Polygon *polygon, Vector position, Vector impulse)
{
    Vector delta_linear_velocity = vector_get_scaled(impulse, 1.0f / polygon->linear_mass);
    polygon->linear_velocity = vector_get_sum(polygon->linear_velocity, delta_linear_velocity);

    Vector local_position = vector_get_difference(position, polygon->position);
    float delta_angular_velocity = vector_get_cross_product(local_position, impulse) / polygon->angular_mass;
    polygon->angular_velocity += delta_angular_velocity;
}

void polygon_copy_collision_parameters(Polygon *polygon, Vector collision_position, Vector collision_direction, float *a, float *b);
