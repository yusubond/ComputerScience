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
@function:Create the socket of tcp or udp;
@param tcp,udp
@time: 8/6 2016
@author: subond
*/
int create_socket(char *opt)
{
  int listenfd,sockfd;
  struct sockaddr_in server,client;
  socklen_t lenserv,lencli;

  if(strcmp(opt, "tcp") == 0)
  {
    //create
    if((listenfd=socket(AF_INET, SOCK_STREAM, 0)) <0)
    {
      perror("Create tcp socket fail.\n");
      return -1;
    }
    lenserv=sizeof(server);
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
  if(strcmp(opt, "udp") == 0)
  {
    if((sockfd = socket(AF_INET,SOCK_DGRAM, 0)) < 0)
    {
      perror("Create udp socket fail!\n");
      return -1;
    }
//    lencli=sizeof(client);
//   client.sin_family = AF_INET;
//    client.sin_port = htons(SERV_UE);
//    client.sin_addr.s_addr = htonl(INADDR_ANY);
//    if(bind(sockfd, (struct sockaddr *)&client, lencli) <0)
//    {
//      perror("Bind socket fail.\n");
//      return -1;
//    }
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
  int tcp_sockfd, udp_sockfd,apt_sockfd;
  struct sockaddr_in server, client;
  socklen_t lenserv, lencli;

  lenserv = sizeof(server);
  lencli = sizeof(client);

  tcp_sockfd = create_socket("tcp");   //tcp
  apt_sockfd = accept(tcp_sockfd, (struct sockaddr *)&client, &lencli);
//  printf("tcp_sockfd is %d\n", tcp_sockfd);
//  printf("apt_sockfd is %d\n", apt_sockfd);
  if(apt_sockfd < 0)
  {
    perror("Accept error");
    exit(1);
  }

  char read_buf[MAX_BUFFER_SIZE],send_buf[MAX_BUFFER_SIZE];
  memset(read_buf, 0x00, MAX_BUFFER_SIZE);
  memset(send_buf, 0x00, MAX_BUFFER_SIZE);

  udp_sockfd = create_socket("udp");
  printf("udp_sockfd is %d\n", udp_sockfd);

  while(1)
  {
    int byt;

    byt = recv(apt_sockfd, read_buf, MAX_BUFFER_SIZE, 0);
    if(byt < 0)
    {
      printf("Read error!\n");
      return -1;
    }
    else if(byt == 0)
    {
      printf("Client connection closed!\n");
      printf("Waiting for the next connection...\n");
      //return 0;
      apt_sockfd = accept(tcp_sockfd, (struct sockaddr *)&client, &lencli);

      if(apt_sockfd < 0)
      {
        perror("Accept error");
        exit(1);
      }
      memset(read_buf, 0x00, MAX_BUFFER_SIZE);
      memset(send_buf, 0x00, MAX_BUFFER_SIZE);

      byt = recv(apt_sockfd, read_buf, MAX_BUFFER_SIZE, 0);
      if(byt < 0)
      {
        printf("Read error!\n");
        return -1;
      }
    }

    printf("The buffer from ctrl is : %s\n", read_buf);
    char str[MAX_BUFFER_SIZE];
    strncpy(str, read_buf+byt-1-19, 19);
    printf("buffer is : %s\n", str);

    char str_tolte[]="WLAN to LTE SWAN-UE";
    char str_toswan[]="LTE to WLAN SWAN-UE";
    if(strcmp(str, str_toswan) == 0)
    {
      udp_to_ue(udp_sockfd, str);
      printf("udp_sockfd is %d\n", udp_sockfd);
    }
    else if(strcmp(str, str_tolte) == 0)
    {
      udp_to_ue(udp_sockfd, str);
      printf("udp_sockfd is %d\n", udp_sockfd);
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
  server.sin_port = htons(SERV_UE);
  inet_pton(AF_INET, "10.103.15.228", &server.sin_addr.s_addr);

  sendto(sockfd, buff, strlen(buff), 0, (struct sockaddr*)&server, lenserv);

//  int recv;
//  recv = recvfrom(sockfd, ue_buff, MAX_BUFFER_SIZE, 0, (struct sockaddr *)&client, &lencli);
//  if(recv < 0)
//  {
//    perror("recvfrom error123.\n");
//    close(sockfd);
//    return -1;
//  }
//  printf("THE BUFFER FROM UE: %s\n", ue_buff);

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
