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
#include <polygon_collider/collision.h>

#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

struct App
{
    Window *window;
    Stopwatch *stopwatch;

    Renderer *renderer;
    Camera *camera;

    Polygon *polygon_left, *polygon_right;

    bool open, freeze;

    Vector contact_point, axis;
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

    app->polygon_left = polygon_create_regular(4, 2.0f, color_get(0.0f, 0.0f, 1.0f));
    polygon_set_orientation(app->polygon_left, 3.14f / 4.0f);
    polygon_set_position(app->polygon_left, vector_get(-5.0f, 0.0f));
    polygon_adjust_linear_velocity(app->polygon_left, vector_get(1.0f, 0.0f));

    app->polygon_right = polygon_create_regular(4, 2.0f, color_get(0.0f, 1.0f, 0.0f));
    polygon_set_orientation(app->polygon_right, 0.0f);
    polygon_set_position(app->polygon_right, vector_get(5.0f, 1.0f));
    polygon_adjust_linear_velocity(app->polygon_right, vector_get(-1.0f, 0.0f));

    return app;
}

void app_destroy(App *app)
{
    polygon_destroy(app->polygon_left);
    app->polygon_left = NULL;

    polygon_destroy(app->polygon_right);
    app->polygon_right = NULL;

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
    app->freeze = false;
    while(app->open)
    {
        float delta_time = stopwatch_get_delta(app->stopwatch);

        app_input(app);
        if(!app->freeze) app_update(app, delta_time);
        app_draw(app);

        if(delta_time < 1 / 60.0f)
            stopwatch_delay(app->stopwatch, 1 / 60.0f - delta_time);
    }
}

static void app_input(App *app)
{
    if(window_was_closed(app->window))
        app->open = false;
}

static void app_update(App *app, float delta_time)
{
    polygon_update(app->polygon_left, delta_time);
    polygon_update(app->polygon_right, delta_time);

    if(collision_check(app->polygon_left, app->polygon_right, &app->contact_point, &app->axis))
    {
        float a_left, b_left, a_right, b_right;
        polygon_copy_collision_parameters(app->polygon_left, app->contact_point, app->axis, &a_left, &b_left);
        polygon_copy_collision_parameters(app->polygon_right, app->contact_point, vector_get_negated(app->axis), &a_right, &b_right);

        // printf("Right parameters: %f %f\n", a_right, b_right);

        float a = a_left + a_right;
        float b = b_left + b_right;

        float impulse = -b / a;
        // printf("Impulse: %f\n", impulse);
        polygon_apply_impulse(app->polygon_left, app->contact_point, vector_get_scaled(app->axis, impulse));
        polygon_apply_impulse(app->polygon_right, app->contact_point, vector_get_scaled(app->axis, -impulse));
    }

    float left_energy = polygon_get_energy(app->polygon_left);
    float right_energy = polygon_get_energy(app->polygon_right);
    float energy = left_energy + right_energy;
    printf("Total energy: %f\n", energy);

    camera_update(app->camera);
}

static void app_draw(App *app)
{
    renderer_submit_polygon(app->renderer, app->polygon_left);
    renderer_submit_polygon(app->renderer, app->polygon_right);
    renderer_submit_vector(app->renderer, app->contact_point, app->axis, color_get(1.0f, 1.0f, 1.0f));

    renderer_flush(app->renderer, app->camera);
    window_refresh(app->window);
}
