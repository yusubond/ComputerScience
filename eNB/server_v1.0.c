/*
* Function:
* 1. 接受来自控制器的消息，并根据消息向UE发送切换指令
*　
***************************************************
* Version   1.0
* Date      8/6 2016
* Author    subond
*
*/

#include "socket_includes.h"
#include <pthread.h>

/*
Function:
1. 创建socket套接字;
2.参数：1-tcp,2-udp
Time: 8/6 2016
Author: subond
*/
int create_socket(int arg)
{
/*
Function:
1. 创建tcp协议的ipv4地址结构的套接字，并绑定和监听;
2. 用于与控制器交互信息;作为服务器端
3. 端口是SERV_PORT_TCP(6777)
*/
  if(arg == 1)
  {
    int listenfd;
    struct sockaddr_in server;
    socklen_t lenserv;
    //create
    if((listenfd=socket(AF_INET, SOCK_STREAM, 0)) <0)
    {
      perror("Create tcp socket fail.\n");
      return -1;
    }
    lenserv = sizeof(server);
    server.sin_family = AF_INET;
    server.sin_port = htons(SERV_PORT_TCP);
    server.sin_addr.s_addr = htonl(INADDR_ANY);
    //bind
    if(bind(listenfd, (struct sockaddr *)&server, lenserv) <0)
    {
      perror("Bind socket fail.\n");
      return -1;
    }
    //listen
    listen(listenfd, MAX_LISTEN_QUE);

    return listenfd;
  }
/*
Function:
1. 创建udp协议的ipv4地址结构的套接字，并绑定地址和端口;
2. 用于与UE交互信息；作为客户端
3. 端口是SERV_PORT_UPD(2345)
*/
  else
  {
    int sockfd;
    struct sockaddr_in client;
    socklen_t lencli;
    lencli=sizeof(client);

    //sockfd = socket(AF_INET,SOCK_DGRAM, 0);
    if((sockfd = socket(AF_INET,SOCK_DGRAM, 0)) < 0)
    {
      perror("Create udp socket fail!\n");
      return -1;
    }

    client.sin_family = AF_INET;
    client.sin_port = htons(SERV_PORT_UDP+1);
    client.sin_addr.s_addr = htonl(INADDR_ANY);
    if(bind(sockfd, (struct sockaddr *)&client, lencli) <0)
    {
      perror("Bind socket fail.\n");
      return -1;
    }

    return sockfd;

  }

}

/*
Function:接受并发送数据
Name:tcp_revsend_data()
Author:subond
Time:1/8 2016
*/

int tcp_revsend_data(void *arg)
{
  int listenfd, sockfd, udp_sockfd;
  struct sockaddr_in server, client;
  socklen_t lenserv, lencli;

  lenserv = sizeof(server);
  lencli = sizeof(client);

  listenfd = create_socket(1);   //tcp
  sockfd = accept(listenfd, (struct sockaddr *)&client, &lencli);
  if(sockfd < 0)
  {
    perror("Accept error");
    exit(1);
  }

  char read_buf[MAX_BUFFER_SIZE],send_buf[MAX_BUFFER_SIZE];
  memset(read_buf, 0x00, MAX_BUFFER_SIZE);
  memset(send_buf, 0x00, MAX_BUFFER_SIZE);

  udp_sockfd = create_socket(2);

  while(1)
  {
    int byt;
    byt = recv(sockfd, read_buf, MAX_BUFFER_SIZE, 0);
    if(byt < 0)
    {
      printf("Read error!\n");
      return -1;
    }
    else if(byt == 0)
    {
      printf("Client connection closed!\n");
      //return 0;
      sockfd = accept(listenfd, (struct sockaddr *)&client, &lencli);
      if(sockfd < 0)
      {
        perror("Accept error");
        exit(1);
      }
      printf("Waiting for the next connection...\n");
      memset(read_buf, 0x00, MAX_BUFFER_SIZE);
      memset(send_buf, 0x00, MAX_BUFFER_SIZE);

      byt = recv(sockfd, read_buf, MAX_BUFFER_SIZE, 0);
      if(byt < 0)
      {
        printf("Read error!\n");
        return -1;
      }
    }

    printf("The buffer from ctrl is : %s\n", read_buf);
    char str[MAX_BUFFER_SIZE];
    strncpy(str, read_buf+19, 11);
    printf("buffer is : %s\n", str);

    char str_tolte[]="swan to lte";
    char str_toswan[]="lte to swan";
    if(strcmp(str, str_toswan) == 0)
    {
      udp_to_ue(udp_sockfd, str);
    }
    else if(strcmp(str, str_tolte) == 0)
    {
      udp_to_ue(udp_sockfd, str);
    }

  }
}
/*
Function:
1. 向UE发送来自控制器的数据，每收到数据便触发发送行为
Author: subond
Time: 8/6 2016
*/
int udp_to_ue(int sockfd, char buff[MAX_BUFFER_SIZE])
{
  struct sockaddr_in server,client;
  socklen_t lenserv,lencli;
  char ue_buff[MAX_BUFFER_SIZE];

  lenserv = sizeof(server);
  lencli = sizeof(client);

  server.sin_family = AF_INET;
  server.sin_port = htons(SERV_PORT_UDP);
  inet_pton(AF_INET, "10.103.14.28", &server.sin_addr.s_addr);

  sendto(sockfd, buff, MAX_BUFFER_SIZE, 0, (struct sockaddr*)&server, lenserv);

  int recv;
  recv = recvfrom(sockfd, ue_buff, MAX_BUFFER_SIZE, 0, (struct sockaddr *)&client, &lencli);
  if(recv < 0)
  {
    perror("recvfrom error.\n");
    close(sockfd);
    return -1;
  }
  printf("THE BUFFER FROM UE: %s\n", ue_buff);

}

int main(int argc, char const *argv[])
{
  pthread_t tid;  //unsigned long int

  int pcreate_id;
  void *ret;
  pcreate_id=pthread_create(&tid, NULL, (void*)&tcp_revsend_data, NULL);
  if(pcreate_id != 0)
  {
    printf("Create pthread error!\n");
    return 1;
  }
//  printf("p_id :%d\n", pcreate_id);
  pthread_join(tid, &ret);

  return 0;

}
