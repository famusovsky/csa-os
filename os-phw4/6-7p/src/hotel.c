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
#define DEFAULT_PORT 8888

void logToDisplay(int display_fd, char *msg, struct sockaddr_in addr) {
  // printf("%s", msg);
  sendMessage(display_fd, msg, addr, sizeof(addr), true);
  memset(msg, 0, MAX_MSG_SIZE);
}

void hotelProcess(int visitors_fd, int street_fd, int display_fd, int rooms_cnt,
                  struct sockaddr_in visitors_addr,
                  struct sockaddr_in street_addr,
                  struct sockaddr_in display_addr) {
  int rooms_days[rooms_cnt];
  int rooms_visitors[rooms_cnt];
  for (int i = 0; i < rooms_cnt; ++i) {
    rooms_days[i] = 0;
    rooms_visitors[i] = -1;
  }

  char buffer[MAX_MSG_SIZE];
  char msg[MAX_MSG_SIZE];
  sendMessage(visitors_fd, "open", visitors_addr, sizeof(visitors_addr), true);
  sprintf(msg, "\nHotel is open\n");
  logToDisplay(display_fd, msg, display_addr);

  while (1) {
    getStringResponce(visitors_fd, visitors_addr, sizeof(visitors_addr), buffer,
                      true);

    if (strcmp(buffer, "all done") == 0) {
      sprintf(msg, "\nThe day is over, Hotel is checking rooms\n");
      logToDisplay(display_fd, msg, display_addr);
      for (int i = 0; i < rooms_cnt; ++i) {
        if (rooms_days[i] > 0) {
          --rooms_days[i];
          if (rooms_days[i] == 0) {
            sprintf(msg,
                    "\nRoom №%d is freed, Visitor №%d is leaving the hotel\n",
                    i + 1, rooms_visitors[i]);
          } else {
            sprintf(msg,
                    "\nRoom №%d is occupied by Visitor №%d, It will be free in "
                    "%d days\n",
                    i + 1, rooms_visitors[i], rooms_days[i]);
          }
          logToDisplay(display_fd, msg, display_addr);
        } else if (rooms_days[i] == 0) {
          sprintf(msg, "\nRooms %d is free\n", i + 1);
          logToDisplay(display_fd, msg, display_addr);
        }
      }
      sendMessage(visitors_fd, "open", visitors_addr, sizeof(visitors_addr),
                  true);
      sprintf(msg, "\nHotel is open\n");
      logToDisplay(display_fd, msg, display_addr);
    }

    if (strcmp(buffer, "visitors cnt") == 0) {
      int cnt = getIntegerResponceOnMessage(
          street_fd, "visitors cnt", street_addr, sizeof(street_addr), true);
      sendIntegerMessage(visitors_fd, cnt, visitors_addr, sizeof(visitors_addr),
                         true);

      sprintf(msg,
              "\nVisitores client is asking for current count of visitors on "
              "the street, It is: %d\n",
              cnt);
      logToDisplay(display_fd, msg, display_addr);
    }

    if (strcmp(buffer, "waiting num days") == 0) {
      sprintf(msg, "\nVisitores client is asking for the info about visitors "
                   "on the street\n");
      logToDisplay(display_fd, msg, display_addr);

      int cnt = getIntegerResponceOnMessage(
          street_fd, "visitors cnt", street_addr, sizeof(street_addr), true);
      sprintf(msg, "\nCurrent count of visitors on the street: %d\n", cnt);
      logToDisplay(display_fd, msg, display_addr);

      sendMessage(street_fd, "waiting num days", street_addr,
                  sizeof(street_addr), true);
      for (int i = 0; i < cnt; ++i) {
        int num = getIntegerResponce(street_fd, street_addr,
                                     sizeof(street_addr), true);
        sendIntegerMessage(visitors_fd, num, visitors_addr,
                           sizeof(visitors_addr), true);
        int days = getIntegerResponce(street_fd, street_addr,
                                      sizeof(street_addr), true);
        sendIntegerMessage(visitors_fd, days, visitors_addr,
                           sizeof(visitors_addr), true);

        sprintf(msg, "\nVisitor №%d wants to stay in the hotel for %d days\n",
                num, days);
        logToDisplay(display_fd, msg, display_addr);
      }
    }

    if (strcmp(buffer, "free room") == 0) {
      sprintf(msg,
              "\nVisitores client is asking for the info about free room\n");
      logToDisplay(display_fd, msg, display_addr);

      bool is_free_found = false;
      for (int i = 0; i < rooms_cnt; ++i) {
        if (rooms_days[i] == 0) {
          sendIntegerMessage(visitors_fd, i, visitors_addr,
                             sizeof(visitors_addr), true);
          is_free_found = true;

          sprintf(msg, "\nRoom №%d is free\n", i);
          logToDisplay(display_fd, msg, display_addr);
          break;
        }
      }
      if (!is_free_found) {
        sendIntegerMessage(visitors_fd, -1, visitors_addr,
                           sizeof(visitors_addr), true);

        sprintf(msg, "\nThere is no free rooms\n");
        logToDisplay(display_fd, msg, display_addr);
      }
    }

    if (strcmp(buffer, "occupy room days num") == 0) {
      int room = getIntegerResponce(visitors_fd, visitors_addr,
                                    sizeof(visitors_addr), true);
      int days = getIntegerResponce(visitors_fd, visitors_addr,
                                    sizeof(visitors_addr), true);
      int num = getIntegerResponce(visitors_fd, visitors_addr,
                                   sizeof(visitors_addr), true);
      rooms_days[room] = days;
      rooms_visitors[room] = num;
      sendMessage(street_fd, "stays num", street_addr, sizeof(street_addr),
                  true);
      sendIntegerMessage(street_fd, num, street_addr, sizeof(street_addr),
                         true);

      sprintf(msg, "\nRoom №%d will be occupied for %d days by Visitor №%d\n",
              room, days, num);
      logToDisplay(display_fd, msg, display_addr);
    }

    if (strcmp(buffer, "the end") == 0) {
      sendMessage(visitors_fd, "the end", visitors_addr, sizeof(visitors_addr),
                  true);
      sendMessage(street_fd, "the end", street_addr, sizeof(street_addr), true);
      sendMessage(display_fd, "the end", display_addr, sizeof(display_addr),
                  true);
      break;
    }
  }
}

int main(int argc, char *argv[]) {
  int hotel_fd;
  int visitors_fd;
  int street_fd;
  int display_fd;
  struct sockaddr_in hotel_addr;
  struct sockaddr_in visitors_addr;
  struct sockaddr_in street_addr;
  struct sockaddr_in display_addr;
  char buffer[MAX_MSG_SIZE] = {0};
  int valread;
  socklen_t addr_len = sizeof(struct sockaddr_in);

  // Create socket file descriptor
  if ((hotel_fd = socket(AF_INET, SOCK_DGRAM, 0)) == 0) {
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

  printf("\nServer started. Waiting for clients...\n");

  while (visitors_fd == 0 || street_fd == 0 || display_fd == 0) {
    // Receive incoming connection
    int client_fd;
    struct sockaddr_in client_addr;
    memset(buffer, 0, MAX_MSG_SIZE);
    if (recvfrom(hotel_fd, buffer, MAX_MSG_SIZE, 0,
                 (struct sockaddr *)&client_addr, &addr_len) < 0) {
      perror("receive failed");
      exit(EXIT_FAILURE);
    }

    printf("\nWaiting for the name...\n");
    if (strcmp(buffer, "visitors") == 0) {
      visitors_fd = hotel_fd;
      visitors_addr = client_addr;
      printf("\nVisitors client connected: %s:%d\n",
             inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));
    } else if (strcmp(buffer, "street") == 0) {
      street_fd = hotel_fd;
      street_addr = client_addr;
      printf("\nStreet client connected: %s:%d\n",
             inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));
    } else if (strcmp(buffer, "display") == 0) {
      display_fd = hotel_fd;
      display_addr = client_addr;
      printf("\nDisplay client connected: %s:%d\n",
             inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));
    } else {
      printf("\nUnknown client connected: %s:%d\n",
             inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));
    }
  }

  // Wait for incoming messages
  hotelProcess(visitors_fd, street_fd, display_fd, 30, visitors_addr,
               street_addr, display_addr);

  // Close connection
  close(hotel_fd);

  printf("\nServer stopped.\n");
  return 0;
}