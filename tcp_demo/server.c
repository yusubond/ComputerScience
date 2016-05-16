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

int main(int argc, char const *argv[])
{
  int sockfd,listenfd;
  struct sockaddr_in server,client;

  //建立socket套接字，用于监听
  listenfd=socket(AF_INET,SOCK_STREAM,0);
  if(listenfd < 0)
  {
    perror("Creatd socket faild!");
    return -1;
  }
  //绑定
  server.sin_family = AF_INET;
  server.sin_port = htons(8888);
  server.sin_addr.s_addr = htonl(INADDR_ANY)
  if(bind(listenfd,(struct sockaddr_in)&server,sizeof(struct sockaddr_in)))
    case /* value */:
  }

  return 0;
}
