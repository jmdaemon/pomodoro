#pragma once
#ifndef POMODORO_TIMER_H
#define POMODORO_TIMER_H

/* Support header include for C++ */
#ifdef __cplusplus
extern "C" {
#endif

#define DEFAULT_NINTERVALS 4

#include <stdlib.h>

typedef struct Timer {
  int shortbreak;
  int longbreak;
  int interval;
  int intervals[4];
} Timer;

int to_secs(int mins);
int workfor(Timer timer, int interval);
Timer init_timer(int sbreak, int lbreak);

#ifdef __cplusplus
}
#endif

#endif // POMODORO_TIMER_H
