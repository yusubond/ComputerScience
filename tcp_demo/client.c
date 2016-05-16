/*
读取服务器时间的小程序_客户端
*/
#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/shm.h>

int main(int argc, char *argv[])
{
  int sockfd = socket(AF_INET,SOCK_STREAM, 0);
  char buf[100];
  int bytes;

  if(sockfd<0)
  {
    perror("socket error");
    return -1;
  }

  struct sockaddr_in servaddr;

  bzero(&servaddr,sizeof(servaddr));
  servaddr.sin_family = AF_INET;
  servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
  servaddr.sin_port = htons(8888);

  if(connect(sockfd,(struct sockaddr*)&servaddr,sizeof(servaddr)) < 0)
  {
    perror("connect error");
    return 0;
  }

  bytes = read(sockfd, buf, 100);
  if(bytes < 0)
  {
    printf("Error, read failed!\n");
    return -1;
  }
  if(bytes == 0)
  {
    printf("Server close connection!\n");
    return -1;
  }
  printf("Read bytes %d\n", bytes);
  printf("Time:%s\n", buf);

  close(sockfd);
  return 0;
}
