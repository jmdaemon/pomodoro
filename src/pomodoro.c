#include "pomodoro.h"

static void error(const char* msg, const char* msg1) {
    fprintf(stderr, "ERROR: %s%s\n", msg, msg1?msg1:"");
    exit(1);
}

/** Read and parse toml config file */
toml_table_t* read_config(char* configfp) {
    FILE* file_contents;
    char errbuf[200];

    file_contents = fopen(configfp, "r");
    if (!file_contents)
      error("Cannot open config.toml - ", strerror(errno));
    toml_table_t* conf = toml_parse_file(file_contents, errbuf, sizeof(errbuf));
    fclose(file_contents);

    if (!conf)
        error("cannot parse - ", errbuf);
    return conf;
}

/** Sets the timer interval periods */
int set_period(const char* units, int value) {
  return (smatch(units, "minutes")) ? value * 60 : value;
}

/** Set the break lengths for the timer */
void set_break(toml_table_t* conf, const char* break_type, const char* units, Timer* timer) {
  toml_datum_t toml_break = toml_int_in(conf, break_type);
  if (toml_break.ok) {
    unsigned int break_length = set_period(units, toml_break.u.i);
    printf("Set %s to: %ld %s\n", break_type, toml_break.u.i, units);

    if (smatch(break_type, "longbreak"))
      timer->longbreak = break_length;
    else if (smatch(break_type, "shortbreak"))
      timer->shortbreak = break_length;
  }
}

/** Parses the user's pomodoro config.toml file
  * The user config will override the timer's default settings
  * If no config file exists then this function will not execute at all
  * The lifetime of the config file is tied to the Pomodoro struct */
toml_table_t* parse_config(char* configfp, Timer *timer) {
  printf("Using custom config file: %s\n", configfp);
  toml_table_t* pomodoro_config = read_config(configfp);
  toml_table_t* pomodoro = toml_table_in(pomodoro_config, "pomodoro");
  toml_datum_t  config = toml_string_in(pomodoro, "config");
  toml_table_t* conf = toml_table_in(pomodoro_config, config.u.s);

  if (!conf) {
    fprintf(stderr, "ERROR: Missing [%s]\n", config.u.s);
    free(config.u.s);
    exit(1);
  }

  toml_datum_t toml_units = toml_string_in(conf, "units");
  const char* units = "";
  if (!toml_units.ok)
      error("Cannot read units. You must provide either 'seconds' or 'minutes'", "");
  else
    units = toml_units.u.s;

  // Set custom intervals for breaks
  set_break(conf, "longbreak", units, timer);
  set_break(conf, "shortbreak", units, timer);

  // Set custom work interval periods
  toml_array_t* intervals = toml_array_in(conf, "intervals");
  if (!intervals)
      error("Cannot read intervals", "");

  // Initialize intervals
  int intvals[4];
  for (int i = 0; ; i++) {
    toml_datum_t intval = toml_int_at(intervals, i);
    if (!intval.ok) break;
    int period = set_period(units, intval.u.i);
    intvals[i] = period;

    if (smatch(units, "minutes"))
      printf("Set interval #%d to: %d %s\n", i + 1, period / 60, units);
    else if (smatch(units, "seconds"))
      printf("Set interval #%d to: %d %s\n", i + 1, period, units);
  }
  
  // Set timer intervals
  for (int i = 0; i < 4; i++)
    (timer->intervals)[i] = intvals[i];

  /* Deallocate */
  free(toml_units.u.s);
  return conf;
}

void show_progress(Pomodoro *pd, int work_period) {
    time_t cur = get_time();
    time_t end = cur + work_period;

    for(int i = 1; i <= work_period; i++) {
        /* Update the current time */
        cur = get_time();
        draw_progress_bar(&pd->pb, cur, end);
        unsigned long progress = (((unsigned long long)i) * pd->pb.maxlength) / work_period;

        /* If the bar hasn't completed yet */
        if (progress != pd->pb.curlength) {
            pd->pb.curlength = progress;
            draw_progress_bar(&pd->pb, cur, end);
        }
        sleep(1);
    }
    puts("");
}

void *stopwatch(void *vargp) {
  Pomodoro *pd = vargp;
  Timer timer = pd->timer;

  while (timer.interval <= 4) {
    /* Start our pomodoro */
    printf("Pomodoro #%d\n", timer.interval);

    /* Work for some time */
    time_t work_period = workfor(timer, timer.interval - 1);
    printf("Working for %ld seconds\n", work_period);

    /* Show a progress bar */
    show_progress(pd, work_period);

    /* Take a break */
    /* If we're on our final pomodoro */
    if (timer.interval == 4) {
      printf("Taking a longer break for %d minutes\n", (timer.longbreak)/60);
      sleep(timer.longbreak); /* Take a longer break */
      timer.interval = 0;
      break; /* And end our pomodoro */
    } else {
      printf("Taking a short break for %d minutes\n", (timer.shortbreak)/60);
      sleep(timer.shortbreak); /* Take a shorter break */
    }
    timer.interval++;

    /* Pause the program and wait for the user to continue */
    if (timer.interval > 0) {
      printf("Hit any key to resume the timer ");
      getchar();
    }
  }
  printf("Pomodoro Interval Complete\n");
  toml_free(pd->config);
  free(pd);
  return NULL;
}
