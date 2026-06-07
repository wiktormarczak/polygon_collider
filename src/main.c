#include <glad/gl.h>
#include <SDL3/SDL.h>
#include <stdbool.h>

int main()
{
    if(!SDL_Init(SDL_INIT_VIDEO))
    {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Could not initialize SDL: %s\n", SDL_GetError());
        return 1;
    }

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

    SDL_Window *window = SDL_CreateWindow("Polygon Collider", 800, 600, SDL_WINDOW_OPENGL);
    if(window == NULL)
    {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Could not create a window: %s\n", SDL_GetError());
        return 1;
    }

    SDL_GLContext context = SDL_GL_CreateContext(window);
    if(context == NULL)
    {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Could not create an OpenGL context: %s\n", SDL_GetError());
        return 1;
    }

    if(!SDL_GL_MakeCurrent(window, context))
    {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Could not make OpenGL context current: %s\n", SDL_GetError());
        return 1;
    }

    if(!gladLoadGL((GLADloadfunc)SDL_GL_GetProcAddress))
    {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Could not initialize OpenGL context.\n");
        return 1;
    }

    bool open = true;
    while(open)
    {
        SDL_Event event;
        while(SDL_PollEvent(&event))
            if(event.type == SDL_EVENT_QUIT)
                open = false;

        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        SDL_GL_SwapWindow(window);
    }

    SDL_GL_DestroyContext(context);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
