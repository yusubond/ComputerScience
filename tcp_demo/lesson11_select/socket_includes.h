
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <netinet/in.h>
#include <sys/shm.h>
#include <arpa/inet.h>

#include <unistd.h>
#include <fcntl.h>

#define SERV_PORT 8888

#define MAX_LISTEN_QUE 5
#define MAX_BUFFER_SIZE 1024

#define RT_ERR (-1)
#define RT_OK 0
