/*
* Linux平台触发条件变量自动复位问题
*/
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
using namespace std;
pthread_cond_t taix;
pthread_mutex_t taximutex;
int tarveler = 0;
void *traveler_arrive(void *name) {
  cout << "Traveler:" << (char *)name << " need a taxi now" << endl;
  pthread_mutex_lock(taximutex);
  traveler++;
  pthread_cond_wait(&taxi, &taximutex);
  pthread_mutex_unlock(&taximutex);
  cout << "Traveler: " << (char *)name << " now got a taxi" << endl;
  pthread_exit(void *)0;
}
void *taxi_arrive(void *name) {
  cout << "Taxi: " << (char *)name << " arrives" << endl;
  while(true) {
    pthread_mutex_lock(&taximutex);
    if(traveler>0) {
      pthread_cond_signal(&taxi);
      pthread_mutex_unlock(&taximutex);
      break;
    }
    pthread_mutex_unlock(&taximutex);
  }
  pthread_exit(void *) 0;
}
int main() {
  taxi = PTHREAD_COND_INITIALIZER;
  taximutex = PTHREAD_MUTEX_INITIALIZER;
  pthread_t pid;
  pthread_arrt_t attr;
  pthread_arrt_init(&attr);
  pthread_create(&pid, &attr, taxi_arrive, (void *)("jack"));
  pthread_create(&pid, &attr, traveler_arrive, (void *)("Susan"));
  pthread_create(&pid, &attr, taxi_arrive, (void *)("Mike"));
  return 0;
}
