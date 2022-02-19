#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>

typedef struct Timer {
  int *shortbreak;
  int *longbreak;
  int *interval_count;
} Timer;

int to_secs(int mins) { return mins * 60; }

void *stopwatch(void *vargp) {
  Timer *timer = vargp;

  while (*timer->interval_count <= 4) {
    if (*timer->interval_count == 4) {
      sleep(*(timer->longbreak));
      (*(timer->interval_count)) = 0;
      printf("Pomodoro #%d\n", *timer->interval_count);
      break;
    } else
      sleep(*timer->shortbreak);

    (*timer->interval_count)++;
    printf("Pomodoro #%d\n", *timer->interval_count);

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

int main() {
  Timer *timer = malloc(sizeof(Timer));
  int shortbreak = to_secs(10);
  int longbreak = to_secs(30);
  /*int shortbreak = 0;*/
  /*int longbreak = 1;*/
  int interval_count = 0;

  timer->shortbreak = &shortbreak;
  timer->longbreak = &longbreak;
  timer->interval_count = &interval_count;

  pthread_t thread_id;
  printf("Starting Pomodoro Timer\n");
  pthread_create(&thread_id, NULL, stopwatch, timer);
  pthread_join(thread_id, NULL);
  exit(0);
}
