#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>

void *threadfn(void *vargp) {
  sleep(1);
  printf("Thread running\n");
  return NULL;
}

int main(int argc, char** arv) {
  pthread_t thread_id;
  printf("Before Thread\n");
  pthread_create(&thread_id, NULL, threadfn, NULL);
  pthread_join(thread_id, NULL);
  printf("After Thread\n");
  exit(0);
}
