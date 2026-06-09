#ifndef STOPWATCH_H
#define STOPWATCH_H

typedef struct Stopwatch Stopwatch;

Stopwatch *stopwatch_create();
void stopwatch_destroy(Stopwatch *stopwatch);

float stopwatch_get_delta(Stopwatch *stopwatch);
void stopwatch_refresh(Stopwatch *stopwatch);

void stopwatch_delay(Stopwatch *stopwatch, float time);

#endif
