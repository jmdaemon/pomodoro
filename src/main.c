#include "pomodoro.h"

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
