#pragma once
#ifndef POMODORO_H
#define POMODORO_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>

typedef struct Timer {
  int *shortbreak;
  int *longbreak;
  int *interval_count;
} Timer;

int to_secs(int mins);
void *stopwatch(void *vargp);
int main();

#endif
