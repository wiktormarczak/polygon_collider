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

#ifndef SHADER_H
#define SHADER_H

unsigned int shader_create_program(const char *vertex_shader_path, const char *fragment_shader_path);

unsigned int shader_create_vertex(const char *shader_path);
unsigned int shader_create_fragment(const char *shader_path);

unsigned int shader_create(const char *shader_path, unsigned int shader_type);

char *shader_get_source(const char *shader_path);

#endif
