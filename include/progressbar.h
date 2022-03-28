#pragma once
#ifndef PROGRESSBAR_H
#define PROGRESSBAR_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>

#define PDT (+5)

typedef struct ProgressBar {
    unsigned long maxlength;
    unsigned long curlength;

    char fillchar;
    char emptychar;
    char leftmargin;
    char rightmargin;

    time_t cur;
    time_t end;
} ProgressBar;

ProgressBar init_progress_bar(unsigned long maxlength, char left, char right, char fill, char empty);
void draw_progress_bar(ProgressBar* p, time_t time_beg, time_t time_end);
time_t get_time();
char* timestamp(time_t rawtime);

#endif
