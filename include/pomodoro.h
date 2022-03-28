#pragma once
#ifndef POMODORO_H
#define POMODORO_H

typedef struct Timer {
  int *shortbreak;
  int *longbreak;
  int *interval_count;
} Timer;

int to_secs(int mins);
void *stopwatch(void *vargp);
int main();

#endif
