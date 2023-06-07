/*
Задача о гостинице – 1.
В гостинице 30 номеров, гости гостиницы снимают номер на одни или несколько
суток. Если в гостинице нет свободных номеров, гости не уходят, а устраиваются
рядом с гостиницей на скамейках и ждут, пока любой из номеров не
освободится. Создать приложение, моделирующее работу гостиницы.
Сервер — это гостиница.
Прибывающие гости могут порождаться отдельным клиентом.
Другой клиент — это скамейки, образующие очередь ожидающих гостей.
*/

#include "funcs_for_udp.h"
#include <arpa/inet.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <time.h>
#include <unistd.h>

#define MAX_MSG_SIZE 1024

void streetProcess(int hotel_sock, int visitors_cnt,
                   struct sockaddr_in hotel_addr) {
  bool are_visitors_wait[visitors_cnt];
  int visitors_days_to_stay[visitors_cnt];
  int waiting_visitors_cnt = visitors_cnt;
  srand(time(NULL));
  for (int i = 0; i < visitors_cnt; ++i) {
    are_visitors_wait[i] = true;
    visitors_days_to_stay[i] = (rand() * (i + 1)) % 10 + 1;
    if (visitors_days_to_stay[i] < 0) {
      visitors_days_to_stay[i] *= -1;
    }

    printf("\nVisitor %d wants to stay in the hotel for %d days\n", i,
           visitors_days_to_stay[i]);
  }

  char buffer[MAX_MSG_SIZE];

  while (true) {
    getStringResponce(hotel_sock, hotel_addr, sizeof(hotel_addr), buffer);

    if (strcmp(buffer, "visitors cnt") == 0) {
      sendIntegerMessage(hotel_sock, waiting_visitors_cnt, hotel_addr,
                         sizeof(hotel_addr));
    }

    if (strcmp(buffer, "waiting num days") == 0) {
      for (int i = 0; i < visitors_cnt; ++i) {
        if (are_visitors_wait[i]) {
          sendIntegerMessage(hotel_sock, i, hotel_addr, sizeof(hotel_addr));
          sendIntegerMessage(hotel_sock, visitors_days_to_stay[i], hotel_addr,
                             sizeof(hotel_addr));
        }
      }
    }

    if (strcmp(buffer, "stays num") == 0) {
      int num = getIntegerResponce(hotel_sock, hotel_addr, sizeof(hotel_addr));
      are_visitors_wait[num] = false;
      waiting_visitors_cnt--;
    }

    if (strcmp(buffer, "the end") == 0) {
      break;
    }
  }
}

int main(int argc, char *argv[]) {
  int hotel_sock = 0;
  struct sockaddr_in serv_addr;
  char buffer[MAX_MSG_SIZE] = {0};

  if (argc != 4) {
    printf("\nUsage: %s <server_ip_address> <server_port> <visitors_cnt>\n",
           argv[0]);
    return -1;
  }

  int visitors_cnt = atoi(argv[3]);

  // Create hotel_socket file descriptor
  if ((hotel_sock = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
    printf("\n hotel_socket creation error \n");
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

  sendMessage(hotel_sock, "street", serv_addr, sizeof(serv_addr));

  streetProcess(hotel_sock, visitors_cnt, serv_addr);

  // Close connection
  close(hotel_sock);

  return 0;
}
