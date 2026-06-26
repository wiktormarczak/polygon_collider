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

#ifndef POLYGON_H
#define POLYGON_H

#include <polygon_collider/geometry/vector.h>
#include <polygon_collider/geometry/edge.h>

typedef struct Polygon Polygon;

Polygon *polygon_create_regular(unsigned int vertex_count, float radius);
void polygon_copy(Polygon *source, Polygon *destination);
void polygon_destroy(Polygon *polygon);

unsigned int polygon_get_vertex_count(Polygon *polygon);

double polygon_get_linear_mass(Polygon *polygon);
double polygon_get_linear_mass_brutally(Polygon *polygon);

double polygon_get_angular_mass(Polygon *polygon);
double polygon_get_angular_mass_brutally(Polygon *polygon);

void polygon_copy_vertex(Polygon *polygon, Vector *vertex_destination);

void polygon_translate(Polygon *polygon, Vector translation);
void polygon_rotate(Polygon *polygon, double rotation);

bool polygon_collision_check(Polygon *left, Polygon *right, Vector *contact_point_destination, Vector *axis_destination);
bool polygon_collision_is_point_inside(Vector point, unsigned int vertex_count, Vector *vertex);
double polygon_collision_get_min_overlap(Polygon *left, Polygon *right, Vector *contact_point_destination, Vector *axis_destination);
double polygon_collision_get_projection_min(Vector axis, Edge edge, Polygon *collision_box, Vector *contact_point_destination);

#endif
