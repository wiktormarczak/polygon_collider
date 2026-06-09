#ifndef WINDOW_H
#define WINDOW_H

#include <stdbool.h>

typedef struct Window Window;

Window *window_create(const char *title, const int width, const int height);
void window_destroy(Window *window);

bool window_was_closed(Window *window);
void window_refresh(Window *window);

#endif
