#include <arpa/inet.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <time.h>
#include <unistd.h>
#include "funcs_for_udp.h"

#define MAX_ECHO_SIZE 1024

void sendMessage(int sock, char *message, struct sockaddr_in other_addr, socklen_t addr_len) {
  sendto(sock, message, strlen(message), 0, (struct sockaddr *)&other_addr, addr_len);
  printf("\n--------------------\n");
  printf("Send message to %d: \n%s", other_addr.sin_port, message);
  printf("\n--------------------\n");

  // just for reading the output easier
  sleep(1);
}

void sendIntegerMessage(int sock, int message, struct sockaddr_in other_addr, socklen_t addr_len) {
  char msg[MAX_ECHO_SIZE];
  sprintf(msg, "%d ", message);
  sendto(sock, msg, strlen(msg), 0, (struct sockaddr *)&other_addr, addr_len);
  printf("\n--------------------\n");
  printf("Send message to %d: \n%s", other_addr.sin_port, msg);
  printf("\n--------------------\n");

  // just for reading the output easier
  sleep(1);
}

void getStringResponce(int sock, struct sockaddr_in other_addr, socklen_t addr_len, char *buffer) {
  memset(buffer, 0, MAX_ECHO_SIZE);
  int valread = recvfrom(sock, buffer, MAX_ECHO_SIZE, 0, (struct sockaddr *)&other_addr, &addr_len);
  if (valread == -1) {
    perror("receive failed");
    exit(EXIT_FAILURE);
  }
  printf("\n--------------------\n");
  printf("Get message from %d: \n%s", other_addr.sin_port, buffer);
  printf("\n--------------------\n");
}

int getIntegerResponce(int sock, struct sockaddr_in other_addr, socklen_t addr_len) {
  char buffer[MAX_ECHO_SIZE];
  getStringResponce(sock, other_addr, addr_len, buffer);
  return atoi(buffer);
}

void getStringResponceOnMessage(int sock, char *message, struct sockaddr_in other_addr, socklen_t addr_len, char *buffer) {
  sendMessage(sock, message, other_addr, addr_len);
  getStringResponce(sock, other_addr, addr_len, buffer);
}

int getIntegerResponceOnMessage(int sock, char *message, struct sockaddr_in other_addr, socklen_t addr_len) {
  sendMessage(sock, message, other_addr, addr_len);
  return getIntegerResponce(sock, other_addr, addr_len);
}
