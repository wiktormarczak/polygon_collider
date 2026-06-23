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
#include <math.h>
#include <string.h>
#include <stdlib.h>

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

void polygon_copy_vertex(Polygon *polygon, Vector *vertex_destination)
{
    memcpy(vertex_destination, polygon->vertex, polygon->vertex_count * sizeof(Vector));
}
