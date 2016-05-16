/*
读取服务器时间的小程序_服务端
*/
#include "socket_includes.h"


int main(int argc, char *argv[])
{
  int listenfd, sockfd;
  struct sockaddr_in server, client;
  char buf[MAX_BUFFER_SIZE], read_buf[MAX_BUFFER_SIZE];
  socklen_t len,length;
  time_t timep;
  //创建套接字
  listenfd = socket(AF_INET,SOCK_DGRAM, 0);
  if(listenfd<0)
  {
    perror("Create socket fail");
    return -1;
  }
  //绑定套接字
  bzero(&server, sizeof(server));
  server.sin_family = AF_INET;
  server.sin_port = htons(8888);                //主机字节序转网络字节序
  server.sin_addr.s_addr = htonl(INADDR_ANY);   //绑定主机的所有网卡

  len = sizeof(server);
  length = sizeof(client);

  if(bind(listenfd, (struct sockaddr*)&server, len) < 0)
  {
    perror("bind error.");
    return -1;
  }
   ssize_t rv;

   memset(buf, 0x00, MAX_BUFFER_SIZE);
   memset(read_buf, 0x00, MAX_BUFFER_SIZE);

  while (1)
  {
    rv = recvfrom(listenfd, read_buf, MAX_BUFFER_SIZE, 0, (struct sockaddr *)&client, &length);
    if(rv < 0)
    {
      printf("recvfrom error.\n");
      close(sockfd);
      return -1;
    }
    printf("len: %d\n", rv);
    printf("client ip: 0x%x, port: %d\n", ntohl(client.sin_addr.s_addr), ntohs(client.sin_port));
    printf("client buf:%s\n", read_buf);

    //timep = time((time_t*)NULL);
    //snprintf(buf, sizeof(buf), "%s", ctime(&timep));

    sendto(listenfd, read_buf, sizeof(read_buf), 0, (struct sockaddr *)&client, length);

    bzero(&client, length);
    memset(buf, 0x00, MAX_BUFFER_SIZE);
    memset(read_buf, 0x00, MAX_BUFFER_SIZE);
  }
  close(sockfd);

  return 0;
}
