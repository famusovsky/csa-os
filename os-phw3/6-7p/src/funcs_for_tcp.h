#include <arpa/inet.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <time.h>
#include <unistd.h>

#define MAX_MSG_SIZE 1024

void sendMessage(int client_fd, char *message, bool do_logging);

void sendIntegerMessage(int client_fd, int message, bool do_logging);

void getStringResponce(int client_fd, char *buffer, bool do_logging);

int getIntegerResponce(int client_fd, bool do_logging);

void getStringResponceOnMessage(int client_fd, char *message, char *buffer, bool do_logging);

int getIntegerResponceOnMessage(int client_fd, char *message, bool do_logging);