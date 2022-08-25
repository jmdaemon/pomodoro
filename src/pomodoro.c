#include "pomodoro.h"

static void error(const char* msg, const char* msg1) {
    fprintf(stderr, "ERROR: %s%s\n", msg, msg1?msg1:"");
    exit(1);
}

toml_table_t* read_config(char* configfp) {
    FILE* file_contents;
    char errbuf[200];

    // Read and parse toml config file
    file_contents = fopen(configfp, "r");
    if (!file_contents) {
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

// Parse the user's config if it exists and
// Override the timer's settings
void parse_config(char* configfp, Timer *timer) {
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
    toml_datum_t lbreak = toml_int_in(conf, "longbreak");
    if (lbreak.ok) {
      timer->longbreak = set_period(units, lbreak.u.i);
      printf("Setting longbreak to: %ld\n", lbreak.u.i);
    }

    toml_datum_t sbreak = toml_int_in(conf, "shortbreak");
    if (sbreak.ok) {
      timer->shortbreak = set_period(units, sbreak.u.i);
      printf("Setting shortbreak to: %ld\n", sbreak.u.i);
    }

    // Set custom work interval periods
    toml_array_t* intervals = toml_array_in(conf, "intervals");
    if (!intervals)
        error("Cannot read intervals", "");

    // Initialize intervals
    int intvals[4];
    for (int i = 0; ; i++) {
      toml_datum_t intval = toml_int_at(intervals, i);
      if (!intval.ok) break;
      int period = set_period(units, (int)intval.u.i);
      intvals[i] = period;
      printf("Setting interval[%d] to: %d\n", i, period);
    }
    
    // Set timer intervals
    for (int i = 0; i < 4; i++)
      (timer->workintervals)[i] = intvals[i];

    free(units.u.s);
    toml_free(conf); }
}

int main() {
  Timer *timer = init_timer(to_secs(10), to_secs(30));

  char* configfp = getenv(POMODORO_CONFIG);
  parse_config(configfp, timer);

  pthread_t thread_id;
  printf("Starting Pomodoro Timer\n");
  pthread_create(&thread_id, NULL, stopwatch, timer);
  pthread_join(thread_id, NULL);
  exit(0);
}
