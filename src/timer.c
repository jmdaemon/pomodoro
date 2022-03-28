#include "timer.h"

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
    printf("Working for %ld\n", work_period);

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
  };


  for (int i = 0; i < 4; i++)
    (timer->workintervals)[i] = intervals[i];
  return timer;
}
