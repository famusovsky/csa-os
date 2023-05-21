#include <arpa/inet.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <time.h>
#include <unistd.h>
#include "funcs_for_tcp.h"

#define MAX_MSG_SIZE 1024

void sendMessage(int client_fd, char *message) {
  send(client_fd, message, MAX_MSG_SIZE, 0);
  printf("\n--------------------\n");
  printf("Send message to %d: \n%s", client_fd, message);
  printf("\n--------------------\n");

  // just for reading the output easier
  sleep(1);
}

void sendIntegerMessage(int client_fd, int message) {
  char msg[MAX_MSG_SIZE];
  sprintf(msg, "%d ", message);
  send(client_fd, msg, MAX_MSG_SIZE, 0);
  printf("\n--------------------\n");
  printf("Send message to %d: \n%s", client_fd, msg);
  printf("\n--------------------\n");
  
  // just for reading the output easier
  sleep(1);
}

void getStringResponce(int client_fd, char *buffer) {
  memset(buffer, 0, MAX_MSG_SIZE);
  int valread = read(client_fd, buffer, MAX_MSG_SIZE);
  if (valread == -1) {
    perror("read failed");
    exit(EXIT_FAILURE);
  }
  printf("\n--------------------\n");
  printf("Get message from %d: \n%s", client_fd, buffer);
  printf("\n--------------------\n");
}

int getIntegerResponce(int client_fd) {
  char buffer[MAX_MSG_SIZE];
  getStringResponce(client_fd, buffer);
  return atoi(buffer);
}

void getStringResponceOnMessage(int client_fd, char *message, char *buffer) {
  sendMessage(client_fd, message);
  getStringResponce(client_fd, buffer);
}

int getIntegerResponceOnMessage(int client_fd, char *message) {
  sendMessage(client_fd, message);
  return getIntegerResponce(client_fd);
}