#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
int value = 5;
int main() {
  for(int i = 0; i < 3; i++) {
    //printf("ID : %d MY MAIN PID: %d\n", i, getpid());
    pid_t fpid = fork();
    if(fpid < 0)
      printf("error in fork()\n");
    else if(fpid == 0) {
      printf("CHILD\n");
    } else if(fpid > 0) {
      printf("PARENT\n");
      printf("\n");
    }
  }
}
