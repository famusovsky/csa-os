#include <arpa/inet.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <time.h>
#include <unistd.h>

#define MAX_MSG_SIZE 1024

void sendMessage(int sock, char *message, struct sockaddr_in other_addr, socklen_t addr_len);

void sendIntegerMessage(int sock, int message, struct sockaddr_in other_addr, socklen_t addr_len);

void getStringResponce(int sock, struct sockaddr_in other_addr, socklen_t addr_len, char *buffer);

int getIntegerResponce(int sock, struct sockaddr_in other_addr, socklen_t addr_len);

void getStringResponceOnMessage(int sock, char *message, struct sockaddr_in other_addr, socklen_t addr_len, char *buffer);

int getIntegerResponceOnMessage(int sock, char *message, struct sockaddr_in other_addr, socklen_t addr_len);