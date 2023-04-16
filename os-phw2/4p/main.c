/*
Задача о гостинице – 1.
В гостинице 30 номеров, клиенты гостиницы снимают номер на одни или несколько
суток. Если в гостинице нет свободных номеров, клиенты не уходят, а устраиваются
рядом с гостиницей на скамейках и ждут, пока любой номеров не освободится.
Создать приложение, моделирующее работу гостиницы. Каждого клиента реализовать
в виде отдельного процесса.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>

const int ROOMS_CNT = 3;

int getSemaphoreSet(int cnt_sems, int sem_key) {
  int sem_id = semget(sem_key, cnt_sems, IPC_CREAT | 0666);
  if (sem_id < 0) {
    perror("Error creating semaphore set: ");
    exit(EXIT_FAILURE);
  }

  for (int i = 0; i < cnt_sems; ++i) {
    if (semctl(sem_id, i, SETVAL, 0) < 0) {
      perror("Error setting semaphore value: ");
      exit(EXIT_FAILURE);
    }
  }

  printf("Set of %d semaphores is created with id: %d\n", cnt_sems, sem_id);
  return sem_id;
}

void eraseSemaphore(int sem_id) {
  printf("Removing semaphore set with id: %d\n", sem_id);

  if (semctl(sem_id, 0, IPC_RMID) < 0) {
    perror("Error removing semaphore set: ");
    exit(EXIT_FAILURE);
  }
}

void runOp(int sem_id, int sem_num, int sem_op, int sem_flg) {
  struct sembuf sb;

  sb.sem_num = sem_num;
  sb.sem_op = sem_op;
  sb.sem_flg = sem_flg;

  if (semop(sem_id, &sb, 1) < 0) {
    perror("Error running semop: ");
    exit(EXIT_FAILURE);
  }
}

// TODO: safe exit on SIGINT and end of the work
void hotel_process(int visitors_sem_id, int rooms_sem_id) {
  printf("Hotel is opened.\n");

  while (1) {
    printf("Waiting for visitors...\n");
    runOp(visitors_sem_id, 1, 0, SEM_UNDO);

    int vsem_val = semctl(visitors_sem_id, 0, GETVAL);
    printf("Count of still waiting visitors: %d\n", vsem_val);

    if (vsem_val == 0) {
      break;
    }

    printf("Checking rooms...\n");
    for (int i = 0; i < ROOMS_CNT; ++i) {
      int room_val = semctl(rooms_sem_id, i, GETVAL);
      if (room_val > 0) {
        runOp(rooms_sem_id, i, -1, 0);
        printf("Room %d will be free in %d days\n", i + 1, room_val);
      } else {
        printf("Room %d is free\n", i + 1);
      }
    }

    printf("All rooms are checked\n");
    runOp(visitors_sem_id, 1, vsem_val, 0);
  }
}

// TODO: safe exit on SIGINT
void visitor_process(int visitors_sem_id, int rooms_sem_id, int num) {
  short is_done = 0;
  srand(time(NULL) + num);
  printf("Visitor %d is created\n", num);

  while (1) {
    printf("Visitor %d is waiting for the hotel\'s opening\n", num);
    while (semctl(visitors_sem_id, 1, GETVAL) == 0) {
      sleep(1);
    }

    printf("Visitor %d is looking for a room\n", num);
    for (int i = 0; i < ROOMS_CNT; ++i) {
      if (semctl(rooms_sem_id, i, GETVAL) == 0) {
        runOp(rooms_sem_id, i, rand() % 7 + 1, 0);
        runOp(visitors_sem_id, 0, -1, 0);
        is_done = 1;
        printf("Visitor %d is staying in the room %d for %d days\n", num, i + 1,
               semctl(rooms_sem_id, i, GETVAL));
        break;
      } else {
        printf("Visitor %d finds out that room %d is busy for %d days\n", num,
               i + 1, semctl(rooms_sem_id, i, GETVAL));
      }
      printf("Visitor %d stops checking room %d\n", num, i + 1);
    }

    printf("Visitor %d\'s day is over\n", num);
    runOp(visitors_sem_id, 1, -1, 0);
    if (is_done) {
      printf("Visitor %d\'s room search is over\n", num);
      break;
    }
  }
}

int main(int argc, char **argv) {
  srand(time(NULL));

  int visitors_sem_id;
  int rooms_sem_id;
  int visitors_cnt = 10;

  if (argc > 1) {
    visitors_cnt = atoi(argv[1]);
  }

  visitors_sem_id = getSemaphoreSet(2, rand() % 10000);
  rooms_sem_id = getSemaphoreSet(ROOMS_CNT, rand() % 10000);

  runOp(visitors_sem_id, 0, visitors_cnt, 0);

  // TODO: safe exit on SIGINT
  for (int i = 0; i < visitors_cnt; ++i) {
    pid_t pid;
    if ((pid = fork()) < 0) {
      perror("Error creating visitor process\n");
      exit(EXIT_FAILURE);
    } else if (pid == 0) {
      visitor_process(visitors_sem_id, rooms_sem_id, i + 1);
      exit(EXIT_SUCCESS);
    }
  }

  hotel_process(visitors_sem_id, rooms_sem_id);

  eraseSemaphore(rooms_sem_id);
  eraseSemaphore(visitors_sem_id);

  printf("Hotel is closed.\n");
  return 0;
}
