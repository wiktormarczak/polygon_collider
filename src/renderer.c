#include <polygon_collider/renderer.h>
#include <glad/gl.h>
#include <SDL3/SDL.h>
#include <stdlib.h>

struct Renderer
{
    SDL_Window *window;
    SDL_GLContext context;
};

Renderer *renderer_create(const char *title, const int width, const int height)
{
    Renderer *renderer = malloc(sizeof(Renderer));

    if(!SDL_Init(SDL_INIT_VIDEO))
    {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Could not initialize SDL: %s\n", SDL_GetError());
        return NULL;
    }

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

    renderer->window = SDL_CreateWindow(title, width, height, SDL_WINDOW_OPENGL);
    if(renderer->window == NULL)
    {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Could not create a window: %s\n", SDL_GetError());
        return NULL;
    }

    renderer->context = SDL_GL_CreateContext(renderer->window);
    if(renderer->context == NULL)
    {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Could not create an OpenGL context: %s\n", SDL_GetError());
        return NULL;
    }

    if(!SDL_GL_MakeCurrent(renderer->window, renderer->context))
    {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Could not make OpenGL context current: %s\n", SDL_GetError());
        return NULL;
    }

    if(!gladLoadGL((GLADloadfunc)SDL_GL_GetProcAddress))
    {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Could not initialize OpenGL context.\n");
        return NULL;
    }

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    return renderer;
}

void renderer_destroy(Renderer *renderer)
{
    SDL_GL_DestroyContext(renderer->context);
    SDL_DestroyWindow(renderer->window);
    SDL_Quit();
}

void renderer_clear(Renderer *renderer)
{
    glClear(GL_COLOR_BUFFER_BIT);
}

void renderer_refresh(Renderer *renderer)
{
    SDL_GL_SwapWindow(renderer->window);
}
