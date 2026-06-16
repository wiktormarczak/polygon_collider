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

void stopwatch_delay(Stopwatch *stopwatch, float time)
{
    SDL_Delay(time * 1000);
}
