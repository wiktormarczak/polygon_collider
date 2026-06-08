#include <polygon_collider/timer.h>
#include <SDL3/SDL.h>
#include <stdlib.h>

struct Timer
{
    float prev_time;
};

Timer *timer_create()
{
    Timer *timer = malloc(sizeof(Timer));

    timer->prev_time = (float)SDL_GetTicks() / 1000.0f;

    return timer;
}

void timer_destroy(Timer *timer)
{
    free(timer);
}

float timer_get_delta(Timer *timer)
{
    float curr_time = (float)SDL_GetTicks() / 1000.0f;
    float delta_time = curr_time - timer->prev_time;
    timer->prev_time = curr_time;
    return delta_time;
}

void timer_refresh(Timer *timer)
{
    timer->prev_time = (float)SDL_GetTicks() / 1000.0f;
}
