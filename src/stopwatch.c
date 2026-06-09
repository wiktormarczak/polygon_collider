#include <polygon_collider/stopwatch.h>
#include <SDL3/SDL.h>
#include <stdlib.h>

struct Stopwatch
{
    float prev_time;
};

Stopwatch *stopwatch_create()
{
    Stopwatch *stopwatch = malloc(sizeof(Stopwatch));

    stopwatch->prev_time = (float)SDL_GetTicks() / 1000.0f;

    return stopwatch;
}

void stopwatch_destroy(Stopwatch *stopwatch)
{
    free(stopwatch);
}

float stopwatch_get_delta(Stopwatch *stopwatch)
{
    float curr_time = (float)SDL_GetTicks() / 1000.0f;
    float delta_time = curr_time - stopwatch->prev_time;
    stopwatch->prev_time = curr_time;
    return delta_time;
}

void stopwatch_refresh(Stopwatch *stopwatch)
{
    stopwatch->prev_time = (float)SDL_GetTicks() / 1000.0f;
}
