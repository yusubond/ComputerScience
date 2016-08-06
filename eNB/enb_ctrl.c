/*
Function:客户端发送消息
Author:subond
Time:1/8 2016
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
  int sockfd;
  struct sockaddr_in server, client;
  socklen_t lenserv, lencli;
  if((sockfd=socket(AF_INET, SOCK_DGRAM, 0)) <0)
  {
    perror("Create UDP socket fail.\n");
    return -1;
  }
//  lenserv = sizeof(server);
  lencli = sizeof(client);
  client.sin_family = AF_INET;
  client.sin_port = htons(SERV_PORT);
  client.sin_addr.s_addr = htonl(INADDR_ANY);
  if(bind(sockfd, (struct sockaddr *)&client, lencli) <0)
  {
    perror("Bind socket fail.\n");
    return -1;
  }
  return sockfd;
}
/*
Function:消息处理函数，实现客户端向服务端发送并接受数据
Author: subond
Time :6/7 2016
*/
int udp_msg_process_serv(int sockfd)
{
  struct sockaddr_in server,client;
  struct sockaddr_in ser;
  socklen_t lenserv,lencli;
  socklen_t len;
  char read_buf[MAX_BUFFER_SIZE];
  char send_buf[MAX_BUFFER_SIZE]="LTE_ping 10.103.14.28";
  char str[20];
  ssize_t rv;
  lencli = sizeof(client);
  lenserv = sizeof(server);
  len = sizeof(ser);

  server.sin_family = AF_INET;
  server.sin_port = htons(SERV_PORT);
  inet_pton(AF_INET, "10.103.15.228", &server.sin_addr.s_addr);
  inet_ntop(AF_INET, &server.sin_addr.s_addr, str, sizeof(str));

  while(1)
  {
    sendto(sockfd, send_buf, strlen(send_buf), 0, (struct sockaddr *)&server, lenserv);
    printf("Send to ctrl(%s): %s\n", str, send_buf);
    sleep(1);
  }


/*
  rv = recvfrom(sockfd, read_buf, MAX_BUFFER_SIZE, 0, (struct sockaddr *)&ser, &len);
  if(rv < 0)
  {
    printf("recvfrom error!\n");
    close(sockfd);
    return -1;
  }
//  struct enb_data *p_rcv;
//  p_rcv=(struct enb_data*)read_buf;
  inet_ntop(AF_INET, &ser.sin_addr.s_addr, str, sizeof(str));
  printf("server buf_back:%s\n", read_buf);
  printf("Server IP: %s, Port: %d\n",str,ntohs(ser.sin_port));

  bzero(&client, lencli);
  bzero(&server, lenserv);
  memset(send_buf, 0x00, MAX_BUFFER_SIZE);
  memset(read_buf, 0x00, MAX_BUFFER_SIZE);
*/
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
    sleep(30);        //延时30s
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
