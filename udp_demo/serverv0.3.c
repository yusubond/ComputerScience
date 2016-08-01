/*
Function:信息回传，并显示客户端IP+Port
Author:subond
Time:5/24 2016
*/
#include "socket_includes.h"
#include <pthread.h>
/*
Function:创建udp协议的ipv4地址结构的套接字
Author:subond
Time :6/7 2016
*/
int udp_creat_socket(void)
{
  int listenfd;
  struct sockaddr_in server, client;
  socklen_t lenserv, lencli;
  if((listenfd=socket(AF_INET, SOCK_DGRAM, 0)) <0)
  {
    perror("Create UDP socket fail.\n");
    return -1;
  }
  lenserv = sizeof(server);
  lencli = sizeof(client);
  server.sin_family = AF_INET;
  server.sin_port = htons(SERV_PORT);
  server.sin_addr.s_addr = htonl(INADDR_ANY);
  if(bind(listenfd, (struct sockaddr *)&server, lenserv) <0)
  {
    perror("Bind socket fail.\n");
    return -1;
  }
  return listenfd;
}
/*
Function:消息处理函数，实现消息回传
Author: subond
Time :6/7 2016
*/
int udp_msg_process_serv(int sockfd)
{
  struct sockaddr_in addrcli;
  socklen_t lencli;
  char read_buf[MAX_BUFFER_SIZE], send_buf[MAX_BUFFER_SIZE];
  char str[20];
  ssize_t rv;
  lencli = sizeof(addrcli);
  if((rv=recvfrom(sockfd, read_buf, MAX_BUFFER_SIZE, 0, (struct sockaddr *)&addrcli, &lencli)) <0)
  {
    perror("recvfrom error.\n");
    close(sockfd);
    return -1;
  }
  inet_ntop(AF_INET, &addrcli.sin_addr.s_addr, str, sizeof(str));
//  printf("size of buf_client: %ld\n", rv);
  printf("client buf:%s\n", read_buf);
  printf("client IP: %s, Port: %d\n", str, ntohs(addrcli.sin_port));
  sendto(sockfd, read_buf, sizeof(read_buf), 0, (struct sockaddr *)&addrcli, lencli);
  bzero(&addrcli, lencli);
  memset(send_buf, 0x00, MAX_BUFFER_SIZE);
  memset(read_buf, 0x00, MAX_BUFFER_SIZE);
}
/*
Name:线程处理函数
Function:1)创建udp套接字，并开启监听模式，阻塞在recvfrom函数
Author:subond
TIme:6/14 2016
*/
void *msg_func(void *arg)
{
  int listenfd;
  listenfd=udp_creat_socket();
  printf("Socket id: %d\n", listenfd);
  while(1)
  {
    udp_msg_process_serv(listenfd);
  }
  close(listenfd);
  return(NULL);
}

int main(int argc, char *argv[])
{
  pthread_t tid;  //unsigned long int
  void *ret;

  int pcreate_id;
  pcreate_id=pthread_create(&tid, NULL, &msg_func, NULL);
  if(pcreate_id != 0)
  {
    printf("Create pthread error!\n");
    return 1;
  }
//  printf("pthread id for create: %lu\n", tid);
//  printf("pcreate id for create: %d, pthread_create() success!\n", pcreate_id);

  pthread_join(tid, &ret);

  return 0;
}
