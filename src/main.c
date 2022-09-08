/*#include <stdio.h>*/
#include "pomodoro.h"
#include <wordexp.h>
#include <linux/limits.h>

bool str_empty(const char* str) {
  bool is_empty = ((str == NULL) || (*str == '\0')) ? true : false;
  return is_empty; 
}

int main() {
  /*Timer *timer = init_timer(to_secs(10), to_secs(30));*/
  Timer timer = init_timer(to_secs(10), to_secs(30));
  ProgressBar pb = init_progress_bar(50, '[', ']', '#', '.');
  toml_table_t* config;

  char* configfp;

  // Allow environment variable to override timer config
  const char* POMODORO_ENV = getenv(POMODORO_CONFIG_ENV_VAR);
  if (!str_empty(POMODORO_ENV)) 
    configfp = (char*) POMODORO_ENV;
  else
    configfp = (char*) POMODORO_DEFAULT_CONFIG_PATH;

  /*printf("%s\n", configfp);*/

  // Expand tilde
  char* expanded;
  /*if (configfp[0] == '~') {*/
  asprintf(&expanded, "%s/%s", getenv("HOME"), configfp);
    /*sprintf(expanded, "%s/%s", );*/
    /*asprintf*/
    
    /*configfp = */
  /*}*/
  printf("expanded: %s\n", expanded);
  
  /*char actualpath [PATH_MAX+1];*/
  /*char *ptr = realpath(configfp, actualpath);*/
  /*printf("%s\n", ptr);*/

  /*if (file_exists(ptr)) */
  if (file_exists(expanded)) 
    config = parse_config(expanded, &timer);
    /*config = parse_config(configfp, timer);*/
  else
    config = NULL;

  Pomodoro pd = {timer, pb, config};
  printf("Starting Pomodoro Timer\n");

  pthread_t thread_id;
  pthread_create(&thread_id, NULL, stopwatch, &pd);
  pthread_join(thread_id, NULL);
  exit(0);
}
