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
#include <toml.h>

/* Constants */
static const char* POMODORO_CONFIG = "POMODORO_CONFIG";

/* Functions */
static void error(const char* msg, const char* msg1);
toml_table_t* read_config(char* configfp);
int set_period(toml_datum_t units, int value);
void parse_config(char* configfp, Timer *timer);

#ifdef __cplusplus
}
#endif

#endif // POMODORO_H
