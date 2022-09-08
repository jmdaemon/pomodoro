#include "pomodoro.h"

bool str_empty(const char* str) {
  bool is_empty = ((str == NULL) || (*str == '\0')) ? true : false;
  return is_empty; 
}

int main() {
  Timer timer = init_timer(to_secs(10), to_secs(30));
  ProgressBar pb = init_progress_bar(50, '[', ']', '#', '.');
  toml_table_t* config;

  char* cfgfp;

  // Allow environment variable to override timer config
  const char* POMODORO_ENV = getenv(POMODORO_CONFIG_ENV_VAR);
  cfgfp = (!str_empty(POMODORO_ENV)) ? (char*) POMODORO_ENV : (char*) POMODORO_DEFAULT_CONFIG_PATH;
  
  // Expand ~/
  char* expanded;
  asprintf(&expanded, "%s/%s", getenv("HOME"), cfgfp);
  config = (file_exists(expanded)) ? parse_config(expanded, &timer) : NULL;

  Pomodoro pd = {timer, pb, config};
  printf("Starting Pomodoro Timer\n");

  // Start timer on new thread
  pthread_t thread_id;
  pthread_create(&thread_id, NULL, stopwatch, &pd);
  pthread_join(thread_id, NULL);
  exit(0);
}
