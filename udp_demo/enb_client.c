#include "socket_includes.h"



int main(int argc, char const *argv[])
{
  int sockfd;
  char buf[MAX_BUFFER_SIZE] = "LTE TO WLAN";
  char read_buf[MAX_BUFFER_SIZE];
  struct sockaddr_in server, ser;
  int rv;
  socklen_t length,len;
  char str[20];

  //struct enb_data* p_rcv;

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
  server.sin_port = htons(2345);
  inet_pton(AF_INET, "192.188.0.2", &server.sin_addr.s_addr);


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
//  printf("***************DATA****************\n");
//  printf("size of buf_server: %d\n",rv);
//  printf("The server IP:%s, and Port:%d\n",str, ntohs(ser.sin_port));
//  printf("subframe %d (%d): eNb %d, aarx %d, rb %d len %d:energy %d (%d dB)\n",p_rcv->enb_subframe,p_rcv->enb_offset,p_rcv->enb_eNB_id,p_rcv->enb_aarx,p_rcv->enb_rb,p_rcv->enb_len,p_rcv->enb_signal_energy_nodc,p_rcv->enb_n0_subband_power_dB);
//  printf("ULSCH MCS: %d\n", p_rcv->mcs1_su);       //默认10
//  printf("DLSCH MCS: %d\n", p_rcv->mcs2_su);       //默认28
//  printf("Number of resource blocks (RB) in UL: %d\n", p_rcv->N_RB_UL);
//  printf("rssi: %d\n", p_rcv->rssi);
  close(sockfd);

  return 0;
}
