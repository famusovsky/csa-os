/*
Задача о гостинице – 1.
В гостинице 30 номеров, клиенты гостиницы снимают номер на одни или несколько
суток. Если в гостинице нет свободных номеров, клиенты не уходят, а устраиваются
рядом с гостиницей на скамейках и ждут, пока любой номеров не освободится.
Создать приложение, моделирующее работу гостиницы. Каждого клиента реализовать
в виде отдельного процесса.
*/

#include <pthread.h>
#include <semaphore.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

const int rooms_cnt = 30;
sem_t visitors_sem[3];
sem_t rooms_sem[30];

// function for hotel work process
void hotel_process() {
  printf("Hotel is opened.\n");

  while (true) {
    printf("Waiting for visitors...\n");

    // wait for all visitors to end their day
    while (sem_getvalue(&visitors_sem[1], 0) > 0) {
      sleep(1);
    }

    int occupied_rooms_cnt = 0;
    printf("Checking rooms...\n");

    // check all rooms if they are free or not
    for (int i = 0; i < rooms_cnt; ++i) {
      int room_val;
      sem_getvalue(&rooms_sem[i], &room_val);
      if (room_val > 0) {
        // if room is occupied, decrease the number of days left for visitor to
        // stay
        sem_wait(&rooms_sem[i]);
        sem_post(&rooms_sem[i]);

        printf("Room %d will be free in %d days\n", i + 1, room_val);
        occupied_rooms_cnt += room_val > 1 ? 1 : 0;
      }
    }

    int waiting_visitors_cnt;
    sem_getvalue(&visitors_sem[0], &waiting_visitors_cnt);
    printf("Count of still waiting visitors: %d\n", waiting_visitors_cnt);

    // if all rooms are free and there are no visitors waiting, close the hotel
    if (waiting_visitors_cnt == 0 && occupied_rooms_cnt == 0) {
      // wait for all visitors to leave
      // runOp(visitors_sem_id, 2, 0, SEM_UNDO);
      while (sem_getvalue(&visitors_sem[2], 0) > 0) {
        sleep(1);
      }
      printf("All visitors are served\n");
      break;
    }

    printf("All rooms are checked\n");
    // runOp(visitors_sem_id, 1, waiting_visitors_cnt, 0);
    for (int i = 0; i < waiting_visitors_cnt; ++i) {
      sem_post(&visitors_sem[1]);
    }
  }
}

// function for generating a list of rooms in random order
void randomiseRoomsCheck(int *rooms, int n) {
  for (int i = 0; i < n; i++) {
    rooms[i] = i;
  }
  for (int i = n - 1; i >= 1; i--) {
    int j = rand() % (i + 1);
    int temp = rooms[i];
    rooms[i] = rooms[j];
    rooms[j] = temp;
  }
}

// function for visitor process
void visitor_process(int num) {
  srand(time(NULL) * num);
  printf("Visitor %d is created\n", num);

  int want_to_stay_for = rand() % 7 + 1;
  int rooms_list[rooms_cnt];
  randomiseRoomsCheck(rooms_list, rooms_cnt);

  while (true) {
    printf("Visitor %d is waiting for the hotel\'s opening\n", num);

    // wait for the hotel to open
    sem_wait(&visitors_sem[1]);

    printf("Visitor %d is looking for a room\n", num);
    for (int j = 0; j < rooms_cnt; ++j) {
      int i = rooms_list[j];

      // check if room is free
      if (sem_getvalue(&rooms_sem[i], 0) == 0) {
        // occupy the room for the number of days visitor wants to stay
        // runOp(rooms_sem_id, i, want_to_stay_for, 0);
        for (int k = 0; k < want_to_stay_for; ++k) {
          sem_post(&rooms_sem[i]);
        }
        // reduce the number of waiting visitors
        // runOp(visitors_sem_id, 0, -1, 0);
        sem_wait(&visitors_sem[0]);
        printf("Visitor %d is staying in the room %d for %d days\n", num, i + 1,
               want_to_stay_for);
        // reduce the number of visitors looking for a room in the moment
        // runOp(visitors_sem_id, 1, -1, 0);
        sem_wait(&visitors_sem[1]);
        // add the number of visitors still in the hotel
        // runOp(visitors_sem_id, 2, 1, 0);
        sem_post(&visitors_sem[2]);
        // wait for the number of days visitor can stay in the hotel to end
        // runOp(rooms_sem_id, i, 0, SEM_UNDO);
        while (sem_getvalue(&rooms_sem[i], 0) > 0) {
          sleep(1);
        }
        // reduce the number of visitors still in the hotel
        // runOp(visitors_sem_id, 2, -1, 0);
        sem_wait(&visitors_sem[2]);
        printf("Visitor %d leaves the hotel\n", num);
        return;
      } else {
        printf("Visitor %d finds out that room %d is busy for %d days\n", num,
               i + 1, sem_getvalue(&rooms_sem[i], 0));
      }
      printf("Visitor %d stops checking room %d\n", num, i + 1);
    }

    printf("Visitor %d\'s day is over\n", num);
    // reduce the number of visitors looking for a room in the moment
    // runOp(visitors_sem_id, 1, -1, 0);
    sem_wait(&visitors_sem[1]);
  }
}

// function for handling SIGINT signal
void sigintHandler(int signum) {
  int status;
  pid_t pid;

  if ((pid = waitpid(-1, &status, 0)) < 0) {
    // Error: this process is a child process
  } else {
    printf("Received SIGINT signal\n");

    // Clean up semaphores
    if (sem_close(visitors_sem) == -1) {
      perror("sem_close");
      exit(EXIT_FAILURE);
    }
    if (sem_close(rooms_sem) == -1) {
      perror("sem_close");
      exit(EXIT_FAILURE);
    }
  }

  exit(signum);
}

int main(int argc, char **argv) {
  // add signal handler for SIGINT
  (void)signal(SIGINT, sigintHandler);
  srand(time(NULL));
  int visitors_cnt = 10;

  if (argc > 1) {
    visitors_cnt = atoi(argv[1]);
  }

  // create visitors semaphore set
  // 0 - number of waiting visitors
  // 1 - number of visitors looking for a room in the moment
  // 2 - number of visitors still in the hotel
  sem_init(&visitors_sem[0], 1, visitors_cnt);
  sem_init(&visitors_sem[1], 1, 0);
  sem_init(&visitors_sem[2], 1, 0);
  // runOp(visitors_sem_id, 0, visitors_cnt, 0);

  // create rooms semaphore set
  // i - number of days left for i-th room to be occupied
  // rooms_sem_id = getSemaphoreSet(rooms_cnt, rand() % 10000);
  for (int i = 0; i < rooms_cnt; ++i) {
    sem_init(&rooms_sem[i], 1, 0);
  }

  // create and start visitors processes
  for (int i = 0; i < visitors_cnt; ++i) {
    pid_t pid;
    if ((pid = fork()) < 0) {
      perror("Error creating visitor process\n");
      exit(EXIT_FAILURE);
    } else if (pid == 0) {
      visitor_process(i + 1);
      exit(EXIT_SUCCESS);
    }
  }

  // start hotel process
  hotel_process();

  // erase semaphores
  if (sem_close(visitors_sem) == -1) {
    perror("sem_close");
    exit(EXIT_FAILURE);
  }
  if (sem_close(rooms_sem) == -1) {
    perror("sem_close");
    exit(EXIT_FAILURE);
  }

  printf("Hotel is closed.\n");
  return 0;
}
