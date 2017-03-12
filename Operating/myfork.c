#include <pthread.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
int value = 0;
void *runner(void *param) {
  value = 5;
  pthread_exit(0);
}
int main() {
  int pid;
  pthread_t tid;
  pthread_attr_t attr;
  pid = fork();
  if(pid == 0) {
    pthread_attr_init(&attr);
    int scope;
    if(pthread_attr_getscope(&attr, &scope) != 0) {
      fprintf(stderr, "Unable to get scheduling scope\n");
    } else {
      if(scope == PTHREAD_SCOPE_PROCESS)
        printf("PTHREAD_SCOPE_PROCESS\n");
      else if(scope == PTHREAD_SCOPE_SYSTEM)
        printf("PTHREAD_SCOPE_SYSTEM\n");
      else
        fprintf(stderr, "Illegal scope value\n");
    }
    int id = pthread_create(&tid, &attr, runner, NULL);
    pthread_join(tid, NULL);
    printf("CHILD: value = %d\n", value);
  } else if(pid > 0) {
    wait(NULL);
    printf("PARENT: value = %d\n", value);
  }
}
