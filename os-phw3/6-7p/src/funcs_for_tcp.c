#include "funcs_for_tcp.h"
#include <arpa/inet.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <time.h>
#include <unistd.h>

#define MAX_MSG_SIZE 1024

void sendMessage(int client_fd, char *message, bool do_logging) {
  send(client_fd, message, MAX_MSG_SIZE, 0);
  if (do_logging) {
    printf("\n--------------------\n");
    printf("Send message to %d: \n%s", client_fd, message);
    printf("\n--------------------\n");
  }

  // just for reading the output easier
  // sleep(1);
}

void sendIntegerMessage(int client_fd, int message, bool do_logging) {
  char msg[MAX_MSG_SIZE];
  sprintf(msg, "%d ", message);
  send(client_fd, msg, MAX_MSG_SIZE, 0);
  if (do_logging) {
    printf("\n--------------------\n");
    printf("Send message to %d: \n%s", client_fd, msg);
    printf("\n--------------------\n");
  }

  // just for reading the output easier
  // sleep(1);
}

void getStringResponce(int client_fd, char *buffer, bool do_logging) {
  memset(buffer, 0, MAX_MSG_SIZE);
  int valread = read(client_fd, buffer, MAX_MSG_SIZE);
  if (valread == -1) {
    perror("read failed");
    exit(EXIT_FAILURE);
  }
  if (do_logging) {
    printf("\n--------------------\n");
    printf("Get message from %d: \n%s", client_fd, buffer);
    printf("\n--------------------\n");
  }
}

int getIntegerResponce(int client_fd, bool do_logging) {
  char buffer[MAX_MSG_SIZE];
  getStringResponce(client_fd, buffer, do_logging);
  return atoi(buffer);
}

void getStringResponceOnMessage(int client_fd, char *message, char *buffer, bool do_logging) {
  sendMessage(client_fd, message, do_logging);
  getStringResponce(client_fd, buffer, do_logging);
}

int getIntegerResponceOnMessage(int client_fd, char *message, bool do_logging) {
  sendMessage(client_fd, message, do_logging);
  return getIntegerResponce(client_fd, do_logging);
}