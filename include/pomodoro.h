#pragma once
#ifndef POMODORO_H
#define POMODORO_H

/* Support header include for C++ */
#ifdef __cplusplus
extern "C" {
#endif

/* Imports */
#include "timer.h"

/* Standard Library */
#include <errno.h>

/* Third Party Libraries */
#include <file.h>
#include <utility.h>
#include <progressbar.h>
#include <toml.h>

/* Constants */
static const char* POMODORO_CONFIG_ENV_VAR = "POMODORO_CONFIG";
static const char* POMODORO_DEFAULT_CONFIG_PATH = "~/.config/pomodoro/config.toml";

typedef struct Pomodoro {
  Timer timer;
  ProgressBar pb;
  toml_table_t* config;
} Pomodoro;

/* Helper Functions */
static void error(const char* msg, const char* msg1);

/* Parsing pomodoro config */
toml_table_t* read_config(char* configfp);
toml_table_t* parse_config(char* configfp, Timer *timer);
int set_period(toml_datum_t units, int value);

void show_progress(Pomodoro *pd, int work_period);

#ifdef __cplusplus
}
#endif

#endif // POMODORO_H
