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

#ifndef RENDERER_H
#define RENDERER_H

#include <polygon_collider/polygon.h>
#include <polygon_collider/camera.h>

typedef struct Renderer Renderer;

Renderer *renderer_create();
void renderer_destroy(Renderer *renderer);

void renderer_submit_polygon(Renderer* renderer, Polygon *polygon);
void renderer_submit_vector(Renderer* renderer, Vector position, Vector direction, Color color);
void renderer_flush(Renderer *renderer, Camera *camera);

#endif
