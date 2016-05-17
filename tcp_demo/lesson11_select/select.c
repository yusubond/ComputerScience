#include "socket_includes.h"

int mz_ipv4_tcp_create_socket(void)
{
  int listenfd, sockfd, opt = 1;
  struct sockaddr_in server,client;
  int bytes = 0;
  socklen_t len;
  time_t timep;
  int ret;

  //创建sockfd套接字
  listenfd = socket(AF_INET,SOCK_STREAM, 0);
  if(listenfd < 0)
  {
    perror("Create socket fail!\n");
    return -1;
  }
  if((ret = setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt))) < 0)
  {
    perror("Error, set socket reuse addr failed!");
    return -1;
  }
  //绑定服务器ip和端口
  bzero(&server, sizeof(server));
  server.sin_family      = AF_INET;
  server.sin_port        = htons(SERV_PORT);
  server.sin_addr.s_addr = htonl(INADDR_ANY);

  len = sizeof(struct sockaddr);
  if(bind(listenfd, (struct sockaddr *)&server, sizeof(server))<0)
  {
    perror("bind error\n");
    return -1;
  }
  listen(listenfd, MAX_LISTEN_QUE);

  return listenfd;
}

int main(int argc, char const *argv[])
{

  int listenfd, sockfd;
  struct sockaddr_in server, client;
  socklen_t len;
  int bytes = 0;
  fd_set global_rdfs, current_rdfs;
  int maxfd;
  int i;
  char buf[MAX_BUFFER_SIZE];

  listenfd = mz_ipv4_tcp_create_socket();
  FD_ZERO(&global_rdfs);
  FD_SET(listenfd, &global_rdfs);
  maxfd = listenfd;
  len = sizeof(client);

  //select
  while (1)
  {
    current_rdfs = global_rdfs;
    if(select(maxfd+1, &current_rdfs, NULL , NULL, NULL) < 0 )
    {
      perror("select error.");
      return RT_ERR;
    }
    for(i = 0; i <= maxfd; i++)
    {
      if(FD_ISSET(i, &current_rdfs))
      {
        if(listenfd == i)
        {
          sockfd = accept(listenfd, (struct sockaddr *)&client, (socklen_t *)&len);
          if(sockfd < 0)
          {
            perror("accept error.");
            return RT_ERR;
          }
          printf("sockfd:%d\n", sockfd);

          FD_CLR(i, &current_rdfs);
          maxfd =  maxfd > sockfd ? maxfd : sockfd;
          FD_SET(sockfd, &global_rdfs);
        }
        else
        {
          printf("read sockfd:%d\n", i);

          bytes = recv(i, buf, MAX_BUFFER_SIZE, 0);
          if(bytes < 0)
          {
            perror("recv error.");
            return RT_ERR;
          }
          if(bytes == 0)
          {
            FD_CLR(i, &global_rdfs);
            close(i);
            continue;
          }
          printf("recv buf: %s\n", buf);
          send(i, buf, strlen(buf), 0);
        }
      }
    }

  }





}
