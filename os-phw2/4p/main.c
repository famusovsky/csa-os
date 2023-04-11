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

const int MSGS_COUNT = 10;
const int BUFSIZE = 256;
#define SEM_KEY 1918

int getSemaphoreSet(int cnt_sems) {
  int sem_id = semget(SEM_KEY, cnt_sems, IPC_CREAT | 0666);
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

  if (semctl(sem_id, IPC_RMID, NULL) < 0) {
    perror("Error removing semaphore: ");
    exit(EXIT_FAILURE);
  }
}

void runOpForAllSemaphoresInSet(int sem_id, int sem_op, int sem_flg) {
  int cnt_sems = semctl(sem_id, 0, GETNCNT);
  struct sembuf sops[cnt_sems];

  for (int i = 0; i < cnt_sems; ++i) {
    sops[i].sem_num = i;
    sops[i].sem_op = sem_op;
    sops[i].sem_flg = sem_flg;
  }

  if (semop(sem_id, sops, 1) < 0) {
    perror("Error running semoperations: ");
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
void hotel_process(int hotel_sem_id, int visitors_sem_id) {
  runOpForAllSemaphoresInSet(hotel_sem_id, 1, 0);

  int sem_val = semctl(visitors_sem_id, 0, GETVAL);
  if (sem_val <= 0) {
    // TODO: end the work
  }
}

// TODO: safe exit on SIGINT
void visitor_process(int hotel_sem_id, int visitors_sem_id) {
  int nsems = semctl(hotel_sem_id, 0, GETNCNT);

  for (int i = 0; i < nsems; ++i) {
    int sem_val = semctl(hotel_sem_id, i, GETVAL);

    if (sem_val > 0) {
      runOp(hotel_sem_id, i, rand() % 7 + 1, 0);
      // TODO: stay in the hotel and leave
      printf("Visitor %d is staying in the room %d", getpid(), i);
      exit(EXIT_SUCCESS);
    }
  }
}

int main(int argc, char **argv) {
  int hotel_sem_id;
  int visitors_sem_id;
  int cnt_rooms = 30;
  int cnt_visitors = 10;

  if (argc > 1) {
    cnt_visitors = atoi(argv[1]);
  }

  hotel_sem_id = getSemaphoreSet(cnt_rooms);
  visitors_sem_id = getSemaphoreSet(1);
  runOp(visitors_sem_id, 0, cnt_visitors, 0);

  srand(time(NULL));
  // TODO: safe exit on SIGINT
  for (int i = 0; i < cnt_visitors; ++i) {
    pid_t pid;
    if ((pid = fork()) < 0) {
      perror("Error creating visitor process\n");
      exit(EXIT_FAILURE);
    } else if (pid == 0) {
      visitor_process(hotel_sem_id, visitors_sem_id);
    }
  }

  hotel_process(hotel_sem_id, visitors_sem_id);

  return 0;
}
