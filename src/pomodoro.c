#include "pomodoro.h"

// TODO:
// Implement progress bar & second by second timer
// Create optional configs and allow users to override timer format

int to_secs(int mins) { return mins * 60; }

int workfor(Timer *timer, int interval) {
  return ((timer->workintervals)[interval]);
}

void *stopwatch(void *vargp) {
  Timer *timer = vargp;

  while (timer->interval_count <= 4) {
    /* Start our pomodoro */
    printf("Pomodoro #%d\n", timer->interval_count);

    /* Work for some time */
    /*printf("Working for %d\n", workfor(timer, timer->interval_count - 1));*/
    sleep(workfor(timer, timer->interval_count - 1));

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

  timer->shortbreak = shortbreak;
  timer->longbreak = longbreak;
  timer->interval_count = interval_count;

  const int intervals[4] = {
    to_secs(20), to_secs(20), to_secs(20), to_secs(20),
    /*4, 4, 4, 4,*/
  };

  for (int i = 0; i < 4; i++)
    (timer->workintervals)[i] = intervals[i];
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
