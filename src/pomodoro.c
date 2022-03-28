#include "pomodoro.h"
#include "progressbar.h"
#include "toml.h"

#include <errno.h>

// TODO:
// Create optional configs and allow users to override timer format

static void error(const char* msg, const char* msg1) {
    fprintf(stderr, "ERROR: %s%s\n", msg, msg1?msg1:"");
    exit(1);
}

int to_secs(int mins) { return mins * 60; }

int workfor(Timer *timer, int interval) {
  return ((timer->workintervals)[interval]);
}

void show_progress(Timer *timer, int work_period) {
    /* Get the current time */
    time_t end = get_time();
    end += work_period;
    time_t cur = get_time();

    for (int i = 1; i <= work_period; i++) {
        sleep(1);
        /* Update the current time */
        cur = get_time();

        draw_progress_bar (&timer->pb, cur, end);
        unsigned long nProgress = ( ((unsigned long long)i) * timer->pb.maxlength) / work_period;
        if (nProgress != timer->pb.curlength) {
            timer->pb.curlength = nProgress;
            draw_progress_bar(&timer->pb, cur, end);
        }
    }
    puts("");
}

void *stopwatch(void *vargp) {
  Timer *timer = vargp;

  while (timer->interval_count <= 4) {
    /* Start our pomodoro */
    printf("Pomodoro #%d\n", timer->interval_count);

    /* Work for some time */
    time_t work_period = workfor(timer, timer->interval_count - 1);
    /*printf("Working for %ld\n", work_period);*/

    /* Show a progress bar */
    show_progress(timer, work_period);

    /* Take a break */
    /* If we're on our final pomodoro */
    if (timer->interval_count == 4) {
      printf("Taking a longer break for %d minutes\n", (timer->longbreak)/60);
      sleep(timer->longbreak); /* Take a longer break */
      timer->interval_count = 0;
      break; /* And end our pomodoro */
    } else {
      printf("Taking a short break for %d minutes\n", (timer->shortbreak)/60);
      sleep(timer->shortbreak); /* Take a shorter break */
    }
    timer->interval_count++;

    /* Pause the program and wait for the user to continue */
    if (timer->interval_count > 0) {
      printf("Hit any key to resume the timer ");
      getchar();
    }
  }
  printf("Pomodoro Interval Complete\n");
  free(timer);
  return NULL;
}

Timer* init_timer(int sbreak, int lbreak) {
  Timer *timer = malloc(sizeof(Timer));
  int shortbreak = sbreak;
  int longbreak = lbreak;
  int interval_count = 1;

  ProgressBar pb = init_progress_bar(50, '[', ']', '#', '.');

  timer->shortbreak = shortbreak;
  timer->longbreak = longbreak;
  timer->interval_count = interval_count;
  timer->pb = pb;

  const int intervals[4] = {
    to_secs(20), to_secs(20), to_secs(20), to_secs(20),
    /*10, 10, 10, 10,*/
  };


  for (int i = 0; i < 4; i++)
    (timer->workintervals)[i] = intervals[i];
  return timer;
}

toml_table_t* read_config(char* configfp) {
    FILE* file_contents;
    char errbuf[200];

    // Read and parse toml config file
    file_contents = fopen(configfp, "r");
    if (!file_contents) {
      /*char* error_msg;*/
      /*sprintf(error_msg, "Cannot open %s - ", configfp);*/
      /*error(error_msg, strerror(errno));*/
      error("Cannot open config.toml - ", strerror(errno));
    }
    toml_table_t* conf = toml_parse_file(file_contents, errbuf, sizeof(errbuf));
    fclose(file_contents);

    if (!conf) {
        error("cannot parse - ", errbuf);
    }
    return conf;
}

int set_period(toml_datum_t units, int value) {
  return ((strcmp(units.u.s, "minutes") == 0) ? value * 60 : value);
}

int main() {
  Timer *timer = init_timer(to_secs(10), to_secs(30));
  /*Timer *timer = init_timer(1, 1);*/

  // Config overrides timer settings
  char* configfp = getenv("POMODORO_CONFIG");
  /*if (configfp != NULL) {*/
  if (configfp) {
    printf("Using custom config file: %s\n", configfp);
    toml_table_t* config = read_config(configfp);

    toml_table_t* conf = toml_table_in(config, "config");
    if (!conf) {
        error("Missing [config]", "");
    }

    toml_datum_t units = toml_string_in(conf, "units");
    if (!units.ok) {
        error("Cannot read units", "");
    }
    printf("Units: %s\n", units.u.s);
    if (strcmp(units.u.s, "minutes") == 0) {
      printf("Using units as minutes.");
    }

    // Set custom intervals for breaks
    /*toml_datum_t lbreak = toml_string_in(conf, "longbreak");*/
    toml_datum_t lbreak = toml_int_in(conf, "longbreak");
    if (lbreak.ok) {
      timer->longbreak = set_period(units, lbreak.u.i);
      /*timer->longbreak = set_period(units, lbreak.u.b);*/
      printf("Setting longbreak to: %ld\n", lbreak.u.i);
      /*printf("Setting longbreak to: %d\n", lbreak.u.b);*/
      /*free(lbreak.u.i);*/
    }
      /*timer->longbreak = (strcmp(units.u.s, "minutes")) ? lbreak.u.i * 60: lbreak.u.i;*/

    /*toml_datum_t sbreak = toml_string_in(conf, "sbreak");*/
    toml_datum_t sbreak = toml_int_in(conf, "shortbreak");
    if (sbreak.ok) {
      /*timer->shortbreak = set_period(units, sbreak.u.i);*/
      timer->shortbreak = set_period(units, sbreak.u.b);
      /*printf("Setting shortbreak to: %ld\n", sbreak.u.i);*/
      printf("Setting shortbreak to: %ld\n", sbreak.u.b);
      /*free(sbreak.u.i);*/
    }
      /*timer->longbreak = (strcmp(units.u.s, "minutes")) ? sbreak.u.i * 60 : sbreak.u.i;*/

    // Set custom work interval periods
    toml_array_t* intervals = toml_array_in(conf, "intervals");
    if (!intervals)
        error("Cannot read intervals", "");

    // Initialize intervals
    int intvals[4];
    for (int i = 0; ; i++) {
      toml_datum_t intval = toml_int_at(intervals, i);
      if (!intval.ok) break;
      /*intvals[i] = (int)intval.u.i;*/
      int period = set_period(units, (int)intval.u.i);
      intvals[i] = period;
      /*printf("Setting interval[%d] to: %ld\n", i, intval.u.i);*/
      printf("Setting interval[%d] to: %d\n", i, period);
    }
    
    // Set timer intervals
    for (int i = 0; i < 4; i++)
      (timer->workintervals)[i] = intvals[i];
    free(units.u.s);
    toml_free(conf);
    /*toml_free(config);*/
  }

  pthread_t thread_id;
  printf("Starting Pomodoro Timer\n");
  pthread_create(&thread_id, NULL, stopwatch, timer);
  pthread_join(thread_id, NULL);
  exit(0);
}
