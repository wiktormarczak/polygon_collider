#ifndef WINDOW_H
#define WINDOW_H

#include <stdbool.h>

typedef struct Window Window;

Window *window_create();
void window_destroy(Window *window);

bool window_was_closed(Window *window);
void window_refresh(Window *window);

#endif
