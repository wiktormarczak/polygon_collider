#ifndef TIMER_H
#define TIMER_H

typedef struct Timer Timer;

Timer *timer_create();
void timer_destroy(Timer *timer);

float timer_get_delta(Timer *timer);
void timer_refresh(Timer *timer);

#endif
