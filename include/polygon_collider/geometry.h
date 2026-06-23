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

#ifndef GEOMETRY_H
#define GEOMETRY_H

#include <polygon_collider/geometry/vector.h>

float geometry_get_linear_mass(unsigned int vertex_count, Vector *vertex);
float geometry_get_linear_mass_brutally(unsigned int vertex_count, Vector *vertex);

float geometry_get_angular_mass(unsigned int vertex_count, Vector *vertex);
float geometry_get_angular_mass_brutally(unsigned int vertex_count, Vector *vertex);

#endif
