#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
void *myfunction(void *arg) {
  for(int i = 0; i < 20; i++) {
    printf("Thread says Hi\n");
    sleep(1);
  }
  return NULL;
}
int main() {
  pthread_t pid;
  if(pthread_create(&pid, NULL, myfunction, NULL)) {
    printf("Error Creating thread\n");
    abort();
  }
  if(pthread_join(pid, NULL)) {
    printf("Error Joining thread\n");
    abort();
  }
  exit(0);
}
