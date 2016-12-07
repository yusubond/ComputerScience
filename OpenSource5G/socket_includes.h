#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/shm.h>

#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>
#include <fcntl.h>


#define MAX_LISTEN_QUE 5
#define SERV_PORT_TCP 6777
#define SERV_PORT_UDP 2819
#define SERV_UE 2345
#define MAX_BUFFER_SIZE 100
