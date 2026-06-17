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

#include <polygon_collider/window.h>
#include <glad/gl.h>
#include <SDL3/SDL.h>
#include <stdlib.h>
#include <stdbool.h>

struct Window
{
    SDL_Window *window;
    SDL_GLContext context;
};

Window *window_create(const char *title, const int width, const int height)
{
    Window *window = malloc(sizeof(Window));

    if(!SDL_Init(SDL_INIT_VIDEO))
    {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Could not initialize SDL: %s\n", SDL_GetError());
        return NULL;
    }

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 16);

    window->window = SDL_CreateWindow(title, width, height, SDL_WINDOW_OPENGL);
    if(window->window == NULL)
    {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Could not create a window: %s\n", SDL_GetError());
        return NULL;
    }

    window->context = SDL_GL_CreateContext(window->window);
    if(window->context == NULL)
    {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Could not create an OpenGL context: %s\n", SDL_GetError());
        return NULL;
    }

    if(!SDL_GL_MakeCurrent(window->window, window->context))
    {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Could not make OpenGL context current: %s\n", SDL_GetError());
        return NULL;
    }

    if(!gladLoadGL((GLADloadfunc)SDL_GL_GetProcAddress))
    {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Could not initialize OpenGL context.\n");
        return NULL;
    }

    glViewport(0, 0, width, height);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glEnable(GL_MULTISAMPLE);

    return window;
}

void window_destroy(Window *window)
{
    SDL_GL_DestroyContext(window->context);
    SDL_DestroyWindow(window->window);
    SDL_Quit();

    free(window);
}

bool window_was_closed(Window *window)
{
    SDL_Event event;
    while(SDL_PollEvent(&event))
        if(event.type == SDL_EVENT_QUIT)
            return true;
    return false;
}

void window_refresh(Window *window)
{
    SDL_GL_SwapWindow(window->window);
}
