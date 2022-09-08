#include "timer.h"

int to_secs(int mins) { return mins * 60; }

int workfor(Timer timer, int interval) {
  return (timer.workintervals[interval]);
}

Timer init_timer(int sbreak, int lbreak) {
  Timer timer = { sbreak, lbreak, 1 };
  const int intervals[DEFAULT_NINTERVALS] = {
    to_secs(20), to_secs(20), to_secs(20), to_secs(20),
  };

  for (int i = 0; i < DEFAULT_NINTERVALS; i++)
    (timer.workintervals)[i] = intervals[i];
  return timer;
}
