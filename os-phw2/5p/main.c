/*
Задача о гостинице – 1.
В гостинице 30 номеров, клиенты гостиницы снимают номер на одни или несколько
суток. Если в гостинице нет свободных номеров, клиенты не уходят, а устраиваются
рядом с гостиницей на скамейках и ждут, пока любой номеров не освободится.
Создать приложение, моделирующее работу гостиницы. Каждого клиента реализовать
в виде отдельного процесса.
*/

#include <fcntl.h>
#include <semaphore.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>

const int rooms_cnt = 30;
const char *visitors_sem_name = "/visitors_sem";
const char *rooms_shm_name = "/rooms_shm";

// create posix named semaphore with given name and value
sem_t *createSemaphores(const char *name, const int value) {
  sem_t *sem = sem_open(name, O_CREAT, 0666, value);
  if (sem == SEM_FAILED) {
    perror("Error creating semaphore: ");
    exit(EXIT_FAILURE);
  }
  return sem;
}

// create shared memory int array with given name and size
int *createSharedMemory(const char *name, const int size) {
  int shm_fd = shm_open(name, O_CREAT | O_RDWR, 0666);
  if (shm_fd < 0) {
    perror("Error creating shared memory: ");
    exit(EXIT_FAILURE);
  }
  if (ftruncate(shm_fd, sizeof(int) * size) < 0) {
    perror("Error truncating shared memory: ");
    exit(EXIT_FAILURE);
  }
  int *shm = mmap(0, sizeof(int) * size, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
  if (shm == MAP_FAILED) {
    perror("Error mapping shared memory: ");
    exit(EXIT_FAILURE);
  }
  for (int i = 0; i < size; ++i) {
    shm[i] = 0;
  }
  return shm;
}

// close posix named semaphore
void eraseSemaphore(const char *name) {
  printf("Removing semaphore set with name: %s\n", name);
  sem_t *sem = sem_open(name, 0);
  if (sem_close(sem) < 0) {
    perror("Error closing semaphore: ");
    exit(EXIT_FAILURE);
  }
  if (sem_unlink(name) < 0) {
    perror("Error removing semaphore set: ");
    exit(EXIT_FAILURE);
  }
}

// close posix shared memory
void eraseSharedMemory(const char *name) {
  printf("Removing shared memory with name: %s\n", name);
  int shm_fd = shm_open(name, O_RDWR, 0666);
  if (shm_fd < 0) {
    perror("Error opening shared memory: ");
    exit(EXIT_FAILURE);
  }
  if (ftruncate(shm_fd, 0) < 0) {
    perror("Error truncating shared memory: ");
    exit(EXIT_FAILURE);
  }
  if (shm_unlink(name) < 0) {
    perror("Error removing shared memory: ");
    exit(EXIT_FAILURE);
  }
  close(shm_fd);
}

// function for hotel work process
void hotel_process() {
  sem_t *visitors_sem = sem_open(visitors_sem_name, 0);
  int *rooms_shm = createSharedMemory(rooms_shm_name, rooms_cnt);

  printf("Hotel is opened.\n");

  while (true) {
    printf("Waiting for visitors...\n");

    // wait for all visitors to end their day
    // runOp(visitors_sem_id, 1, 0, SEM_UNDO);
    while (sem_getvalue(&visitors_sem[1], 0) > 0) {
      sleep(1);
    }

    int occupied_rooms_cnt = 0;
    printf("Checking rooms...\n");

    // check all rooms if they are free or not
    for (int i = 0; i < rooms_cnt; ++i) {
      int room_val = rooms_shm[i];
      if (room_val > 0) {
        // if room is occupied, decrease the number of days left for visitor to
        // stay
        --rooms_shm[i];
        printf("Room %d will be free in %d days\n", i + 1, room_val);
        occupied_rooms_cnt += room_val > 1 ? 1 : 0;
      }
    }

    int waiting_visitors_cnt = sem_getvalue(&visitors_sem[0], 0);
    printf("Count of still waiting visitors: %d\n", waiting_visitors_cnt);

    // if all rooms are free and there are no visitors waiting, close the hotel
    if (waiting_visitors_cnt == 0 && occupied_rooms_cnt == 0) {
      // wait for all visitors to leave
      while (sem_getvalue(&visitors_sem[2], 0) > 0) {
        sleep(1);
      }
      printf("All visitors are served\n");
      break;
    }

    printf("All rooms are checked\n");
    for (int i = 0; i < waiting_visitors_cnt; ++i) {
      sem_post(&visitors_sem[1]);
    }
  }
}

// function for generating a list of rooms in random order
void randomiseRoomsCheck(int *rooms, const int n) {
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
void visitor_process(const int num) {
  sem_t *visitors_sem = sem_open(visitors_sem_name, 0);
  int *rooms_shm = createSharedMemory(rooms_shm_name, rooms_cnt);
  srand(time(NULL) * num);
  printf("Visitor %d is created\n", num);

  int want_to_stay_for = rand() % 7 + 1;
  int rooms_list[rooms_cnt];
  randomiseRoomsCheck(rooms_list, rooms_cnt);

  while (true) {
    printf("Visitor %d is waiting for the hotel\'s opening\n", num);

    // wait for the hotel to open
    // while (semctl(visitors_sem_id, 1, GETVAL) == 0) {
    //   sleep(1);
    // }
    sem_wait(&visitors_sem[1]);

    printf("Visitor %d is looking for a room\n", num);
    for (int j = 0; j < rooms_cnt; ++j) {
      int i = rooms_list[j];

      // check if room is free
      if (rooms_shm[i] == 0) {
        // occupy the room for the number of days visitor wants to stay
        // runOp(rooms_sem_id, i, want_to_stay_for, 0);
        rooms_shm[i] = want_to_stay_for;
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
        while (rooms_shm[i] > 0) {
          sleep(1);
        }
        // reduce the number of visitors still in the hotel
        // runOp(visitors_sem_id, 2, -1, 0);
        sem_wait(&visitors_sem[2]);
        printf("Visitor %d leaves the hotel\n", num);
        return;
      } else {
        printf("Visitor %d finds out that room %d is busy for %d days\n", num,
               i + 1, rooms_shm[i]);
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
    eraseSemaphore(visitors_sem_name);
    eraseSharedMemory(rooms_shm_name);
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
  // visitors_sem_id = getSemaphoreSet(3, rand() % 10000);
  sem_t *visitors_sem = createSemaphores(visitors_sem_name, 3);
  // runOp(visitors_sem_id, 0, visitors_cnt, 0);
  for (int i = 0; i < visitors_cnt; ++i) {
    sem_post(&visitors_sem[0]);
  }

  // create rooms semaphore set
  // i - number of days left for i-th room to be occupied
  // rooms_sem_id = getSemaphoreSet(rooms_cnt, rand() % 10000);
  int *rooms_shm = createSharedMemory(rooms_shm_name, rooms_cnt);

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

  // erase semaphores and shared memory
  eraseSemaphore(visitors_sem_name);
  eraseSharedMemory(rooms_shm_name);

  printf("Hotel is closed.\n");
  return 0;
}
