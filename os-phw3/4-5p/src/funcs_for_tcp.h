#include <arpa/inet.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <time.h>
#include <unistd.h>

#define MAX_MSG_SIZE 1024

void sendMessage(int client_fd, char *message);

void sendIntegerMessage(int client_fd, int message);

void getStringResponce(int client_fd, char *buffer);

int getIntegerResponce(int client_fd);

void getStringResponceOnMessage(int client_fd, char *message, char *buffer);

int getIntegerResponceOnMessage(int client_fd, char *message);