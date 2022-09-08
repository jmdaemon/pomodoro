#include "timer.h"

#define DEFAULT_INTERVAL_LENGTH 4

int to_secs(int mins) { return mins * 60; }

/*
int workfor(Timer *timer, int interval) {
  return ((timer->workintervals)[interval]);
}
*/

int workfor(Timer timer, int interval) {
  return (timer.workintervals[interval]);
}

Timer init_timer(int sbreak, int lbreak) {
  /*Timer *timer = malloc(sizeof(Timer));*/
  Timer timer = { sbreak, lbreak, 1 };
  /*const int DEFAULT_INTERVAL_LENGTH = 4;*/
  /*const int intervals[4] = {*/
  const int intervals[DEFAULT_INTERVAL_LENGTH] = {
    to_secs(20), to_secs(20), to_secs(20), to_secs(20),
  };
  for (int i = 0; i < DEFAULT_INTERVAL_LENGTH; i++)
    (timer.workintervals)[i] = intervals[i];
  return timer;
}


  /*for (int i = 0; i < 4; i++)*/
    /*(timer->workintervals)[i] = intervals[i];*/


  /*int shortbreak = sbreak;*/
  /*int longbreak = lbreak;*/
  /*int interval_count = 1;*/

  /*ProgressBar pb = init_progress_bar(50, '[', ']', '#', '.');*/

  /*timer->shortbreak = shortbreak;*/
  /*timer->longbreak = longbreak;*/
  /*timer->interval_count = interval_count;*/
  /*timer->pb = pb;*/

  /*const int intervals[4] = {*/
    /*to_secs(20), to_secs(20), to_secs(20), to_secs(20),*/
  /*};*/


  /*for (int i = 0; i < 4; i++)*/
    /*(timer->workintervals)[i] = intervals[i];*/

  
