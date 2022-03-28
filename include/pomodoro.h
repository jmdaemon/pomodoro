#pragma once
#ifndef POMODORO_H
#define POMODORO_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include "progressbar.h"

typedef struct Timer {
  int shortbreak;
  int longbreak;
  int interval_count;
  ProgressBar pb;
  int workintervals[4];
} Timer;

int main();
int to_secs(int mins);
void *stopwatch(void *vargp);

int workfor(Timer *timer, int interval);
void show_progress(Timer *timer, int work_period);
Timer* init_timer(int sbreak, int lbreak);

#endif
