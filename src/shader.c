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

#include <polygon_collider/shader.h>
#include <glad/gl.h>
#include <SDL3/SDL_log.h>
#include <stdio.h>
#include <stdlib.h>

unsigned int shader_create_program(const char *vertex_shader_path, const char *fragment_shader_path)
{
    unsigned int vertex_shader = shader_create_vertex(vertex_shader_path);
    unsigned int fragment_shader = shader_create_fragment(fragment_shader_path);

    unsigned int shader_program = glCreateProgram();

    glAttachShader(shader_program, vertex_shader);
    glAttachShader(shader_program, fragment_shader);

    glLinkProgram(shader_program);

    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);

    int status;
    glGetProgramiv(shader_program, GL_LINK_STATUS, &status);
    if(!status)
    {
        char info_log[1024];
        glGetProgramInfoLog(shader_program, 1024, NULL, info_log);
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Could not link the shader program:\n%s\n", info_log);
        return 0;
    }

    return shader_program;
}

unsigned int shader_create_vertex(const char *shader_path)
{
    return shader_create(shader_path, GL_VERTEX_SHADER);
}

unsigned int shader_create_fragment(const char *shader_path)
{
    return shader_create(shader_path, GL_FRAGMENT_SHADER);
}

unsigned int shader_create(const char *shader_path, unsigned int shader_type)
{
    char *source = shader_get_source(shader_path);

    unsigned int shader = glCreateShader(shader_type);
    glShaderSource(shader, 1, (const char **)&source, NULL);
    glCompileShader(shader);

    free(source);

    int status;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
    if(!status)
    {
        char info_log[1024];
        glGetShaderInfoLog(shader, 1024, NULL, info_log);
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Could not compile the shader from file %s:\n%s\n", shader_path, info_log);
        return 0;
    }

    return shader;
}

char *shader_get_source(const char *shader_path)
{
    FILE *file = fopen(shader_path, "rb");

    if(!file)
    {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Could not open the shader source file %s.\n", shader_path);
        return NULL;
    }

    fseek(file, 0, SEEK_END);
    unsigned int size = ftell(file);
    fseek(file, 0, SEEK_SET);

    char *source = malloc(size + 1);
    fread(source, 1, size, file);
    source[size] = '\0';

    fclose(file);

    return source;
}
