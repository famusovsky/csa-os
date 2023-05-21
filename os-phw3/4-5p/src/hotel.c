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
#define DEFAULT_PORT 8888

void hotelProcess(int visitors_fd, int street_fd, int rooms_cnt) {
  int rooms_days[rooms_cnt];
  int rooms_visitors[rooms_cnt];
  for (int i = 0; i < rooms_cnt; ++i) {
    rooms_days[i] = 0;
    rooms_visitors[i] = -1;
  }

  char buffer[MAX_MSG_SIZE];
  sendMessage(visitors_fd, "open");
  printf("\nHotel is open\n");

  while (1) {
    getStringResponce(visitors_fd, buffer);

    if (strcmp(buffer, "all done") == 0) {
      for (int i = 0; i < rooms_cnt; ++i) {
        if (rooms_days[i] > 0) {
          --rooms_days[i];
          if (rooms_days[i] == 0) {
            printf("\nRoom №%d is freed, Visitor №%d is leaving the hotel\n", i + 1, rooms_visitors[i]);
          } else {
            printf("\nRoom №%d is occupied by Visitor №%d will be free in %d days\n", i + 1, rooms_visitors[i], rooms_days[i]);
          }
        } else if (rooms_days[i] == 0) {
          printf("\nRooms %d is free\n", i + 1);
        }
      }
      sendMessage(visitors_fd, "open");
    }

    if (strcmp(buffer, "visitors cnt") == 0) {
      int cnt = getIntegerResponceOnMessage(street_fd, "visitors cnt");
      sendIntegerMessage(visitors_fd, cnt);
    }

    if (strcmp(buffer, "waiting num days") == 0) {
      int cnt = getIntegerResponceOnMessage(street_fd, "visitors cnt");
      sendMessage(street_fd, "waiting num days");
      for (int i = 0; i < cnt * 2; ++i) {
        int num = getIntegerResponce(street_fd);
        sendIntegerMessage(visitors_fd, num);
      }
    }

    if (strcmp(buffer, "free room") == 0) {
      bool is_free_found = false;
      for (int i = 0; i < rooms_cnt; ++i) {
        if (rooms_days[i] == 0) {
          sendIntegerMessage(visitors_fd, i);
          is_free_found = true;
          break;
        }
      }
      if (!is_free_found) {
        sendIntegerMessage(visitors_fd, -1);
      }
    }

    if (strcmp(buffer, "occupy room days num") == 0) {
      int room = getIntegerResponce(visitors_fd);
      int days = getIntegerResponce(visitors_fd);
      int num = getIntegerResponce(visitors_fd);
      rooms_days[room] = days;
      rooms_visitors[room] = num;
      sendMessage(street_fd, "stays num");
      sendIntegerMessage(street_fd, num);
    }

    if (strcmp(buffer, "the end") == 0) {
      sendMessage(visitors_fd, "the end");
      sendMessage(street_fd, "the end");
      break;
    }
  }
}

int main(int argc, char *argv[]) {
  int hotel_fd;
  int visitors_fd;
  int street_fd;
  struct sockaddr_in hotel_addr;
  struct sockaddr_in visitors_addr;
  struct sockaddr_in street_addr;
  char buffer[MAX_MSG_SIZE] = {0};
  int valread;

  // Create socket file descriptor
  if ((hotel_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
    perror("socket failed");
    exit(EXIT_FAILURE);
  }

  // Set server address and port
  hotel_addr.sin_family = AF_INET;
  hotel_addr.sin_addr.s_addr = INADDR_ANY;

  if (argc != 2) {
    hotel_addr.sin_port = htons(DEFAULT_PORT);
  } else {
    hotel_addr.sin_port = htons(atoi(argv[1]));
  }

  // Bind socket to address and port
  if (bind(hotel_fd, (struct sockaddr *)&hotel_addr, sizeof(hotel_addr)) < 0) {
    perror("bind failed");
    exit(EXIT_FAILURE);
  }

  printf("\nServer\'s ip address: %s\n", inet_ntoa(hotel_addr.sin_addr));
  printf("\nServer\'s port: %d\n", ntohs(hotel_addr.sin_port));

  // Listen for incoming connections
  if (listen(hotel_fd, 3) < 0) {
    perror("listen failed");
    exit(EXIT_FAILURE);
  }

  printf("\nServer started. Waiting for clients...\n");

  while (visitors_fd == 0 || street_fd == 0) {
    // Accept incoming connection
    int client_fd;
    struct sockaddr_in client_addr;
    if ((client_fd = accept(hotel_fd, (struct sockaddr *)&client_addr,
                            (socklen_t *)&client_addr)) < 0) {
      perror("accept failed");
      exit(EXIT_FAILURE);
    }
    printf("\nWaiting for the name...\n");
    getStringResponce(client_fd, buffer);
    if (strcmp(buffer, "visitors") == 0) {
      visitors_fd = client_fd;
      visitors_addr = client_addr;
      printf("\nVisitors client connected: %s:%d\n",
             inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));
    } else if (strcmp(buffer, "street") == 0) {
      street_fd = client_fd;
      street_addr = client_addr;
      printf("\nStreet client connected: %s:%d\n",
             inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));
    } else {
      printf("\nUnknown client connected: %s:%d\n",
             inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));
    }
  }

  // Wait for incoming messages
  hotelProcess(visitors_fd, street_fd, 30);

  // Close connection
  close(visitors_fd);
  close(street_fd);
  close(hotel_fd);

  printf("\nServer stopped.\n");
  return 0;
}
