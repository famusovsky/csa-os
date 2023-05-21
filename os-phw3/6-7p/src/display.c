/*
Задача о гостинице – 1.
В гостинице 30 номеров, гости гостиницы снимают номер на одни или несколько суток. 
Если в гостинице нет свободных номеров, гости не уходят, а устраиваются
рядом с гостиницей на скамейках и ждут, пока любой из номеров не
освободится. Создать приложение, моделирующее работу гостиницы. 
Сервер — это гостиница. 
Прибывающие гости могут порождаться отдельным клиентом. 
Другой клиент — это скамейки, образующие очередь ожидающих гостей.
*/

#include <arpa/inet.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/_types/_null.h>
#include <sys/socket.h>
#include <time.h>
#include <unistd.h>
#include "funcs_for_tcp.h"

#define MAX_MSG_SIZE 1024

void displayProcess(int serv_sock) {
  char buffer[MAX_MSG_SIZE];
  while (true) {
    getStringResponce(serv_sock, buffer, false);

    printf("%s\n", buffer);

    if (strcmp(buffer, "the end") == 0) {
      break;
    }
  }
}

int main(int argc, char *argv[]) {
  int serv_sock = 0;
  struct sockaddr_in serv_addr;
  char buffer[MAX_MSG_SIZE] = {0};

  if (argc != 3) {
    printf("\nUsage: %s <server_ip_address> <server_port>\n", argv[0]);
    return -1;
  }

  // Create serv_socket file descriptor
  if ((serv_sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
    printf("\n serv_socket creation error \n");
    return -1;
  }

  // Set server address and port
  memset(&serv_addr, '0', sizeof(serv_addr));
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_port = htons(atoi(argv[2]));

  // Convert IPv4 and IPv6 addresses from text to binary form

  if (inet_pton(AF_INET, argv[1], &serv_addr.sin_addr) <= 0) {
    printf("\nInvalid address/ Address not supported \n");
    return -1;
  }

  // Connect to server
  if (connect(serv_sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
    printf("\nConnection Failed \n");
    return -1;
  }

  sendMessage(serv_sock, "display", true);

  displayProcess(serv_sock);

  // Close connection
  close(serv_sock);

  return 0;
}
