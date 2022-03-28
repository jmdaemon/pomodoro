#include "pomodoro.h"

// TODO:
// Implement progress bar & second by second timer
// Create optional configs and allow users to override timer format

int to_secs(int mins) { return mins * 60; }

void *stopwatch(void *vargp) {
  Timer *timer = vargp;

  while (*timer->interval_count <= 4) {
    if (*timer->interval_count == 4) {
      sleep(*(timer->longbreak));
      printf("Pomodoro #%d\n", *timer->interval_count);
      (*(timer->interval_count)) = 0;
      break;
    } else {
      sleep(*timer->shortbreak);
      printf("Pomodoro #%d\n", *timer->interval_count);
    }
    (*timer->interval_count)++;

    // Pause the program and wait for the user to continue
    if (*timer->interval_count > 0) {
      printf("Hit any key to resume the timer ");
      char c = getchar();
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

  timer->shortbreak = &shortbreak;
  timer->longbreak = &longbreak;
  timer->interval_count = &interval_count;
  return timer;
}

int main() {
  Timer *timer = init_timer(to_secs(10), to_secs(30));
  /*Timer *timer = init_timer(1, 1);*/

  pthread_t thread_id;
  printf("Starting Pomodoro Timer\n");
  pthread_create(&thread_id, NULL, stopwatch, timer);
  pthread_join(thread_id, NULL);
  exit(0);
}
