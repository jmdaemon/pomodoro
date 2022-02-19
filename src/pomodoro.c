#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

typedef struct Timer {
  int shortbreak;
  int longbreak;
  int interval_count;

  /*int shortbreak = to_secs(10);*/
  /*int longbreak = to_secs(30);*/
} Timer;

int to_secs(int mins) { return mins * 60; }

void *threadfn(void *vargp) {
  Timer *timer = malloc(sizeof(vargp));
  /*sleep(1);*/
  /*(timer->interval_count == 4) ? sleep(timer->longbreak) :
   * sleep(timer->shortbreak);*/
  while (timer->interval_count <= 4) {
    if (timer->interval_count == 4) {
      sleep(timer->longbreak);
      timer->interval_count = 0;
      printf("Pomodoro #%d\n", timer->interval_count);
      break;
    } else
      sleep(timer->shortbreak);

    timer->interval_count++;
    printf("Pomodoro #%d\n", timer->interval_count);
  }
  printf("Pomodoro Interval Complete\n");
  free(timer);
  return NULL;
}

int main(int argc, char **argv) {
  /*Timer timer = { to_secs(10), to_secs(30) };*/
  Timer *timer = malloc(sizeof(Timer));

  pthread_t thread_id;
  printf("Before Thread\n");
  pthread_create(&thread_id, NULL, threadfn, timer);
  pthread_join(thread_id, NULL);
  printf("After Thread\n");
  exit(0);
}
