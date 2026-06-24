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

#ifndef POLYGON_OBJECT_H
#define POLYGON_OBJECT_H

#include <polygon_collider/geometry/vector.h>
#include <polygon_collider/color.h>

typedef struct PolygonObject PolygonObject;

PolygonObject *polygon_object_create(unsigned int vertex_count, Vector *vertex, Color color);
PolygonObject *polygon_object_create_regular(unsigned int vertex_count, double radius, Color color);

void polygon_object_destroy(PolygonObject *polygon_object);

unsigned int polygon_object_get_vertex_count(PolygonObject *polygon_object);
Color polygon_object_get_color(PolygonObject *polygon_object);
void polygon_object_copy_world_vertex(PolygonObject *polygon_object, Vector *destination);
double polygon_object_get_energy(PolygonObject *polygon_object);
double polygon_object_get_angular_velocity(PolygonObject *polygon_object);

void polygon_object_set_position(PolygonObject *polygon_object, Vector position);
void polygon_object_translate(PolygonObject *polygon_object, Vector translation);
void polygon_object_set_orientation(PolygonObject *polygon_object, double orientation);
void polygon_object_adjust_linear_velocity(PolygonObject *polygon_object, Vector delta);
void polygon_object_adjust_angular_velocity(PolygonObject *polygon_object, double delta);
void polygon_object_set_color(PolygonObject *polygon_object, Color color);

void polygon_object_update(PolygonObject *polygon_object, double delta_time);

void polygon_object_apply_impulse(PolygonObject *polygon_object, Vector position, Vector impulse);
void polygon_object_copy_collision_parameters(PolygonObject *polygon_object, Vector collision_position, Vector collision_direction, double *a, double *b);

#endif
