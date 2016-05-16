/*
读取服务器时间的小程序_服务端
*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>

#include <sys/shm.h>
#include <sys/types.h>
#include <sys/socket.h>

#define MAX_LISTEN_QUEUE 10

int main(int argc, char const *argv[])
{
  int listenfd, sockfd;
  struct sockaddr_in server, client;
  char buf[200];
  socklen_t len;
  int timep;

  listenfd = socket(AF_INET,SOCK_STREAM, 0);
  if(listenfd<0)
  {
    perror("Create socket fail");
    return -1;
  }

  bzero(&server, sizeof(server));
  server.sin_family = AF_INET;
  server.sin_port = htons(8888);                //主机字节序转网络字节序
  server.sin_addr.s_addr = htonl(INADDR_ANY);   //绑定主机的所有网卡

  len=sizeof(struct sockaddr);
  if(bind(listenfd, (struct sockaddr*)&server, len) < 0)
  {
    perror("bind error.");
    return -1;
  }

  listen(listenfd, MAX_LISTEN_QUEUE);

  while(1)
  {
    sockfd = accept(listenfd, (struct sockaddr*)&client, &len);
    if(sockfd < 0)
    {
      perror("accept error.");
      return -1;
    }

    timep = time(NULL);
    snprintf(buf, sizeof(buf), "%d",ctime(&timep));
    write(sockfd, buf, strlen(buf));
    printf("Bytes:%s\n",buf);

    close(sockfd);
  }

  return 0;
}
