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

#include <polygon_collider/vector.h>
#include <polygon_collider/color.h>

typedef struct Polygon Polygon;

Polygon *polygon_create(unsigned int vertex_count, Vector *vertex, Color color);
Polygon *polygon_create_regular(unsigned int vertex_count, float radius, Color color);

void polygon_destroy(Polygon *polygon);

unsigned int polygon_get_vertex_count(Polygon *polygon);
Color polygon_get_color(Polygon *polygon);
void polygon_copy_world_vertex(Polygon *polygon, Vector *destination);
float polygon_get_energy(Polygon *polygon);

void polygon_set_position(Polygon *polygon, Vector position);
void polygon_set_orientation(Polygon *polygon, float orientation);
void polygon_adjust_linear_velocity(Polygon *polygon, Vector delta);
void polygon_adjust_angular_velocity(Polygon *polygon, float delta);
void polygon_set_color(Polygon *polygon, Color color);

void polygon_update(Polygon *polygon, float delta_time);

void polygon_apply_impulse(Polygon *polygon, Vector position, Vector impulse);
void polygon_copy_collision_parameters(Polygon *polygon, Vector collision_position, Vector collision_direction, float *a, float *b);

#endif
