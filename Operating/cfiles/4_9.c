/*
*Author:subond
* Time: 2017-03-10
* Function: 用户在命令行输入一个数字，然后创建一个独立线程来输出小于或等于输入数的所有素数
*/
#include <pthread.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <math.h>
int n;
void *runner(void *param) {
  if(n <= 1) return(NULL);
  for(int i = 2; i <= n; i++) {
    int len = sqrt(i * 1.0);
    int flag = 1;
    for(int j = 2; j <= len; j++) {
      if(i % j == 0)
        flag = 0;
    }
    if(flag == 1)
      printf("%d ", i);
  }
  pthread_exit(0);
}
int main() {
  printf("Enter a number(>= 0):");
  scanf("%d", &n);
  int pid;
  pthread_t tid;
  pthread_attr_t attr;
  pthread_attr_init(&attr);
  pid = pthread_create(&tid, &attr, runner, NULL);
  if(pid != 0) {
    printf("Create pthread error\n");
    return 1;
  }
  pthread_join(tid, NULL);
  return 0;
}
