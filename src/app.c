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

#define TITLE "Polygon Collider"
#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

#include <polygon_collider/app.h>
#include <polygon_collider/window.h>
#include <polygon_collider/stopwatch.h>
#include <polygon_collider/renderer.h>
#include <polygon_collider/camera.h>
#include <polygon_collider/polygon.h>

#include <stdbool.h>
#include <stdlib.h>
#include <time.h>

struct App
{
    Window *window;
    Stopwatch *stopwatch;

    Renderer *renderer;
    Camera *camera;

    unsigned int polygon_count;
    Polygon **polygon;

    bool open;
};

static void app_input(App *app);
static void app_update(App *app, float delta_time);
static void app_draw(App *app);

App *app_create()
{
    srand(time(NULL));

    App *app = malloc(sizeof(App));

    app->window = window_create(TITLE, WINDOW_WIDTH, WINDOW_HEIGHT);
    app->stopwatch = stopwatch_create();

    app->renderer = renderer_create();
    app->camera = camera_create((float)800 / (float)600);

    app->polygon_count = 8192;
    app->polygon = malloc(app->polygon_count * sizeof(Polygon *));

    for(int i = 0; i < app->polygon_count; i++)
    {
        app->polygon[i] = polygon_create_regular(3 + rand() % 6, 1.0f, color_get((rand() % 11) / 10.0f, (rand() % 11) / 10.0f, (rand() %11) / 10.0f));
        polygon_adjust_linear_velocity(app->polygon[i], vector_get((rand() % 11 - 5) / 10.0f, (rand() % 11 - 5) / 10.0f));
        polygon_adjust_angular_velocity(app->polygon[i], (rand() % 11 - 5) / 10.0f);
    }

    return app;
}

void app_destroy(App *app)
{
    int i;
    for(i = 0; i < app->polygon_count; i++)
    {
        polygon_destroy(app->polygon[i]);
        app->polygon[i] = NULL;
    }

    free(app->polygon);
    app->polygon = NULL;

    app->polygon_count = 0;

    camera_destroy(app->camera);
    app->camera = NULL;

    renderer_destroy(app->renderer);
    app->renderer = NULL;

    stopwatch_destroy(app->stopwatch);
    app->stopwatch = NULL;

    window_destroy(app->window);
    app->window = NULL;

    free(app);
}

void app_run(App *app)
{
    app->open = true;
    while(app->open)
    {
        float delta_time = stopwatch_get_delta(app->stopwatch);

        app_input(app);
        app_update(app, delta_time);
        app_draw(app);
    }
}

static void app_input(App *app)
{
    if(window_was_closed(app->window))
        app->open = false;
}

static void app_update(App *app, float delta_time)
{
    for(int i = 0; i < app->polygon_count; i++)
        polygon_update(app->polygon[i], delta_time);

    camera_update(app->camera);
}

static void app_draw(App *app)
{
    for(int i = 0; i < app->polygon_count; i++)
        renderer_submit(app->renderer, app->polygon[i]);

    renderer_flush(app->renderer, app->camera);
    window_refresh(app->window);
}
