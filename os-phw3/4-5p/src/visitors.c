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

void waitForHotelToOpen(int hotel_sock) {
  char buffer[MAX_MSG_SIZE];
  
  printf("\nWaiting for the hotel to open\n");
  getStringResponce(hotel_sock, buffer);
  while (strcmp(buffer, "open") != 0) {
    sleep(1);
    getStringResponce(hotel_sock, buffer);
    printf("\nWaiting for the hotel to open\n");
  }
}

void visitorsProcess(int hotel_sock) {
  waitForHotelToOpen(hotel_sock);

  int visitors_cnt = getIntegerResponceOnMessage(hotel_sock, "visitors cnt");
  int inrooms_visitors_list[visitors_cnt];
  char buffer[MAX_MSG_SIZE];
  for (int i = 0; i < visitors_cnt; ++i) {
    srand(time(NULL));
    inrooms_visitors_list[i] = -1;
  }
  printf("\nAll visitors count equels %d\n", visitors_cnt);
  
  while (true) {
    visitors_cnt = getIntegerResponceOnMessage(hotel_sock, "visitors cnt");
    if (visitors_cnt == 0) {
      printf("\nAll visitors left the hotel\n");
      sendMessage(hotel_sock, "the end");
      break;
    }

    int waiting_visitors_nums[visitors_cnt];
    int waiting_visitors_days_to_stay[visitors_cnt];
    sendMessage(hotel_sock, "waiting num days");
    for (int i = 0; i < visitors_cnt; ++i) {
      waiting_visitors_nums[i] = getIntegerResponce(hotel_sock);
      waiting_visitors_days_to_stay[i] = getIntegerResponce(hotel_sock);
      printf("\nVisitor %d wants to stay in the hotel for %d days\n", waiting_visitors_nums[i], waiting_visitors_days_to_stay[i]);
    }
    
    printf("\n%d visitors are waiting for the hotel\'s opening\n", visitors_cnt);

    for (int i = 0; i < visitors_cnt; ++i) {
      if (inrooms_visitors_list[i] > 0) {
        --inrooms_visitors_list[i];
        printf("\nVisitor %d will be free in %d days\n", i + 1, inrooms_visitors_list[i]);
      } else if (inrooms_visitors_list[i] == 0) {
        printf("\nVisitor %d is leaving the hotel\n", i + 1);
        inrooms_visitors_list[i] = -1;
      }
    } 

    int free_room = getIntegerResponceOnMessage(hotel_sock, "free room");
    for (int i = 0; i < visitors_cnt && free_room >= 0; ++i, free_room = getIntegerResponceOnMessage(hotel_sock, "free room")) {
      sendMessage(hotel_sock, "occupy room days num");
      sendIntegerMessage(hotel_sock, free_room);
      sendIntegerMessage(hotel_sock, waiting_visitors_days_to_stay[i]);
      sendIntegerMessage(hotel_sock, i);
      printf("\nVisitor %d is staying in the room %d for %d days\n", i, free_room, waiting_visitors_days_to_stay[i]);
      inrooms_visitors_list[i] = waiting_visitors_days_to_stay[i];
    }

    sendMessage(hotel_sock, "all done");

    // wait for the hotel to open
    waitForHotelToOpen(hotel_sock);
  }
}

int main(int argc, char *argv[]) {
  int hotel_sock = 0;
  struct sockaddr_in hotel_addr;
  char buffer[MAX_MSG_SIZE] = {0};

  if (argc != 3) {
    printf("\nUsage: %s <hotel_ip_address> <hotel_port>\n", argv[0]);
    return -1;
  }

  // Create socket file descriptor
  if ((hotel_sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
    printf("\n Socket creation error \n");
    return -1;
  }

  // Set hotel address and port
  memset(&hotel_addr, '0', sizeof(hotel_addr));
  hotel_addr.sin_family = AF_INET;
  hotel_addr.sin_port = htons(atoi(argv[2]));
  
  // Convert IPv4 and IPv6 addresses from text to binary form
  if (inet_pton(AF_INET, argv[1], &hotel_addr.sin_addr) <= 0) {
    printf("\nInvalid address/ Address not supported \n");
    return -1;
  }

  // Connect to hotel
  if (connect(hotel_sock, (struct sockaddr *)&hotel_addr, sizeof(hotel_addr)) < 0) {
    printf("\nConnection Failed \n");
    return -1;
  }

  sleep(3);

  sendMessage(hotel_sock, "visitors");

  visitorsProcess(hotel_sock);

  // Close connection
  close(hotel_sock);

  return 0;
}
