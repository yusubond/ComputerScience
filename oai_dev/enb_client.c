#include "socket_includes.h"



int main(int argc, char const *argv[])
{
  int sockfd;
  char buf[MAX_BUFFER_SIZE] = "LTE TO WLAN";
  char read_buf[MAX_BUFFER_SIZE];
  struct sockaddr_in server, ser, client;
  int rv;
  socklen_t length,len,lencli;
  char str[20];

  //struct enb_data* p_rcv;

  bzero(read_buf, MAX_BUFFER_SIZE);
  bzero(&server, sizeof(server));
  len = sizeof(ser);
  length = sizeof(server);
  lencli = sizeof(client);

  //sockfd = socket(AF_INET,SOCK_DGRAM, 0);
  if((sockfd = socket(AF_INET,SOCK_DGRAM, 0)) < 0)
  {
    perror("Create socket fail!\n");
    return -1;
  }

  client.sin_family = AF_INET;
  client.sin_port = htons(SERV_PORT);
  client.sin_addr.s_addr = htonl(INADDR_ANY);
  if(bind(sockfd, (struct sockaddr *)&client, lencli) <0)
  {
    perror("Bind socket fail.\n");
    return -1;
  }


  server.sin_family = AF_INET;
  server.sin_port = htons(SERV_PORT);
  inet_pton(AF_INET, "10.103.12.228", &server.sin_addr.s_addr);


  sendto(sockfd, buf, strlen(buf), 0, (struct sockaddr *)&server, length);


  rv = recvfrom(sockfd, read_buf, MAX_BUFFER_SIZE, 0, (struct sockaddr *)&ser, &len);
  if(rv < 0)
  {
    printf("recvfr0m error!\n");
    close(sockfd);
    return -1;
  }
  struct enb_data *p_rcv;
  p_rcv=(struct enb_data*)read_buf;
  inet_ntop(AF_INET, &ser.sin_addr.s_addr, str, sizeof(str));
  printf("server buf_back:%s\n", read_buf);
  printf("Server IP: %s, Port: %d\n",str,ntohs(ser.sin_port));
  close(sockfd);

  return 0;
}
