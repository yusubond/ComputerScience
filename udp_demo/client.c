#include "socket_includes.h"



int main(int argc, char const *argv[])
{
  int sockfd;
  char buf[MAX_BUFFER_SIZE] = "Hello UDP!";
  char read_buf[MAX_BUFFER_SIZE];
  struct sockaddr_in server, ser;
  int rv;
  socklen_t length,len;

  bzero(read_buf, MAX_BUFFER_SIZE);
  bzero(&server, sizeof(server));
  len = sizeof(ser);
  length = sizeof(server);

  //sockfd = socket(AF_INET,SOCK_DGRAM, 0);
  if((sockfd = socket(AF_INET,SOCK_DGRAM, 0)) < 0)
  {
    perror("Create socket fail!\n");
    return -1;
  }


  server.sin_family = AF_INET;
  server.sin_port = htons(8888);
  inet_pton(AF_INET, "10.103.14.28", &server.sin_addr.s_addr);


  sendto(sockfd, buf, strlen(buf), 0, (struct sockaddr *)&server, length);


  rv = recvfrom(sockfd, read_buf, MAX_BUFFER_SIZE, 0, (struct sockaddr *)&ser, &len);
  if(rv < 0)
  {
    printf("recvfr0m error!\n");
    close(sockfd);
    return -1;
  }
  printf("server buf_back:%s\n", read_buf);
  close(sockfd);

  return 0;
}
