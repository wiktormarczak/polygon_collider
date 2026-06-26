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


#include <polygon_collider/app.h>
#include <SDL3/SDL_main.h>
#include <stddef.h>

int main()
{
    App *app = app_create();
    if(app == NULL)
        return 1;

    app_run(app);

    app_destroy(app);
    app = NULL;

    return 0;
}
