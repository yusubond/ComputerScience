#include "socket_includes.h"

int mz_ipv4_tcp_create_socket(void)
{
  int listenfd, sockfd, opt = 1;
  struct sockaddr_in server,client;
  char buf[200],read_buf[200];
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
  server.sin_family = AF_INET;
  server.sin_port = htons(SERV_PORT);
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

int mz_process_data(int sockfd)
{
  char buf[MAX_BUFFER_SIZE], read_buf[MAX_BUFFER_SIZE];

  memset(read_buf, 0x00, MAX_BUFFER_SIZE);
  memset(buf, 0x00, MAX_BUFFER_SIZE);
  while(1)
  {
    int bytes;
    printf("Bytes:%d\n", strlen(read_buf));

    bytes = recv(sockfd, read_buf, MAX_BUFFER_SIZE, 0);
    if(bytes < 0)
    {
      printf("read err!\n");
      return -1;
    }
    if(bytes == 0)
    {
      printf("client connection closed!\n");
      return 0;
    }

    send(sockfd, read_buf, bytes, 0);

    if(!strcmp(read_buf, "q"))
    {
      return 0;
    }

  }
  close(sockfd);
  return 0;
}

int main(int argc, char const *argv[])
{

  int listenfd, sockfd;
  struct sockaddr_in server, client;
  socklen_t len;


  len = sizeof(client);

  listenfd = mz_ipv4_tcp_create_socket();
  //接受请求
  while(1)
  {
    sockfd = accept(listenfd, (struct sockaddr *)&client, &len);
    if(accept(listenfd, (struct sockaddr *)&client, &len) < 0)
    {
      perror("accept error");
      exit(1);
    }

    printf("sockfd=%d\n", sockfd);
    printf("IP:0x%x, Port:%d\n", ntohl(client.sin_addr.s_addr), ntohs(client.sin_port));

    if(fork() == 0)
    {
      close(listenfd);
      mz_process_data(sockfd);
      exit(0);
    }
    close(sockfd);
  }

  return 0;
}
