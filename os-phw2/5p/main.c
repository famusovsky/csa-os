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
#include <signal.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

#define ROOMS_CNT 30

const char *rooms_shm_name = "/rooms_sem";
const char *check_sem_name = "/check_sem";
const char *visitors_sem_name = "/visitors_sem";
sem_t *visitor_sem[3];
sem_t *check_sem;
int rooms_shm_fd;

// create posix named semaphore set
void createSemaphoreSet(sem_t **sems_ptr, const int sems_cnt,
                        const char *sem_name) {
  for (int i = 0; i < sems_cnt; ++i) {
    char current_name[sizeof(sem_name) + 1];
    sprintf(current_name, "%s%d", sem_name, i);
    sems_ptr[i] = sem_open(current_name, O_CREAT, 0666, 1);
    if (sems_ptr[i] == SEM_FAILED) {
      perror("Error creating semaphore: ");
      exit(EXIT_FAILURE);
    }
  }

  printf("Semaphore set with common name %s created\n", sem_name);
}

// erase posix named semaphore set
void eraseSemaphoreSet(sem_t **sems_ptr, const int sems_cnt,
                       const char *sem_name) {
  printf("Removing semaphore set with common name: %s\n", sem_name);
  for (int i = 0; i < sems_cnt; i++) {
    char current_name[sizeof(sem_name) + 1];
    sprintf(current_name, "%s%d", sem_name, i);
    if (sem_unlink(current_name) < 0) {
      perror("Error unlinking semaphore: ");
      exit(EXIT_FAILURE);
    }
  }
  for (int i = 0; i < sems_cnt; i++) {
    if (sem_close(sems_ptr[i]) < 0) {
      perror("Error closing semaphore: ");
      exit(EXIT_FAILURE);
    }
  }
}

// create posix named shared memory
int createSharedMemory(const char *shm_name, const int shm_size) {
  int *addr;
  int shm;

  // create shared memory object
  if ((shm = shm_open(shm_name, O_RDWR, 0666)) == -1) {
    printf("Error creating shared memory object: %s\n", shm_name);
    perror("shm_open");
    return NULL;
  }

  // set size of shared memory object
  addr = mmap(0, shm_size, PROT_WRITE | PROT_READ, MAP_SHARED, shm, 0);
  if (addr == MAP_FAILED) {
    printf("Error getting pointer to shared memory\n");
    perror("mmap");
    return NULL;
  }

  for (int i = 0; i < shm_size; ++i) {
    addr[i] = 0;
  }

  printf("Shared memory object with name %s created\n", shm_name);
  return shm;
}

// open posix named shared memory
int *openSharedMemory(const char *shm_name, const int shm_size) {
  int *addr;
  int shm;

  // open shared memory object
  if ((shm = shm_open(shm_name, O_RDWR, 0666)) == -1) {
    printf("Error opening shared memory object: %s\n", shm_name);
    perror("shm_open");
    return NULL;
  }

  // set size of shared memory object
  addr = mmap(0, shm_size, PROT_WRITE | PROT_READ, MAP_SHARED, shm, 0);
  if (addr == MAP_FAILED) {
    printf("Error getting pointer to shared memory\n");
    perror("mmap");
    return NULL;
  }

  printf("Shared memory object with name %s opened\n", shm_name);
  return addr;
}

// function for hotel work process
void hotel_process() {
  printf("Hotel is opened.\n");
  int *rooms_shm = openSharedMemory(rooms_shm_name, ROOMS_CNT);

  while (true) {
    printf("Waiting for visitors...\n");

    // wait for all visitors to end their day
    while (sem_getvalue(visitor_sem[1], NULL) != 0) {
      sleep(1);
    }

    int occupied_ROOMS_CNT = 0;
    printf("Checking rooms...\n");

    // check all rooms if they are free or not
    for (int i = 0; i < ROOMS_CNT; ++i) {
      int room_val = rooms_shm[i];
      if (room_val > 0) {
        // if room is occupied, decrease the number of days left for visitor to
        // stay
        rooms_shm[i]--;
        printf("Room %d will be free in %d days\n", i + 1, room_val);
        occupied_ROOMS_CNT += room_val > 1 ? 1 : 0;
      }
    }

    int waiting_visitors_cnt = sem_getvalue(visitor_sem[0], NULL);
    printf("Count of still waiting visitors: %d\n", waiting_visitors_cnt);

    // if all rooms are free and there are no visitors waiting, close the hotel
    if (waiting_visitors_cnt == 0 && occupied_ROOMS_CNT == 0) {
      // wait for all visitors to leave
      while (sem_getvalue(visitor_sem[2], NULL) != 0) {
        sleep(1);
      }
      printf("All visitors are served\n");
      break;
    }

    printf("All rooms are checked\n");
    for (int i = 0; i < waiting_visitors_cnt; ++i) {
      sem_post(visitor_sem[1]);
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
  int *rooms_shm = openSharedMemory(rooms_shm_name, ROOMS_CNT);
  srand(time(NULL) * num);
  printf("Visitor %d is created\n", num);

  int want_to_stay_for = rand() % 7 + 1;
  int rooms_list[ROOMS_CNT];
  randomiseRoomsCheck(rooms_list, ROOMS_CNT);

  while (true) {
    printf("Visitor %d is waiting for the hotel\'s opening\n", num);

    // wait for the hotel to open
    while (sem_getvalue(visitor_sem[1], NULL) == 0) {
      sleep(1);
    }

    printf("Visitor %d is looking for a room\n", num);
    for (int j = 0; j < ROOMS_CNT; ++j) {
      int i = rooms_list[j];

      // check if room is free
      sem_wait(check_sem);
      if (rooms_shm[i] == 0) {
        sem_post(check_sem);
        // occupy the room for the number of days visitor wants to stay
        rooms_shm[i] += want_to_stay_for;
        // reduce the number of waiting visitors
        sem_wait(visitor_sem[0]);
        printf("Visitor %d is staying in the room %d for %d days\n", num, i + 1,
               want_to_stay_for);
        // reduce the number of visitors looking for a room in the moment
        sem_wait(visitor_sem[1]);
        // add the number of visitors still in the hotel
        sem_post(visitor_sem[2]);
        // wait for the number of days visitor can stay in the hotel to end
        while (rooms_shm[i] != 0) {
          sleep(1);
        }
        // reduce the number of visitors still in the hotel
        sem_wait(visitor_sem[2]);
        printf("Visitor %d leaves the hotel\n", num);
        return;
      } else {
        sem_post(check_sem);
        printf("Visitor %d finds out that room %d is busy for %d days\n", num,
               i + 1, rooms_shm[i]);
      }
      printf("Visitor %d stops checking room %d\n", num, i + 1);
    }

    printf("Visitor %d\'s day is over\n", num);
    // reduce the number of visitors looking for a room in the moment
    sem_wait(visitor_sem[1]);
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

    // Clean up semaphores and shared memory
    eraseSemaphoreSet(visitor_sem, 3, visitors_sem_name);
    eraseSemaphoreSet(&check_sem, 1, check_sem_name);
    shm_unlink(rooms_shm_name);
    close(rooms_shm_fd);
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
  createSemaphoreSet(visitor_sem, 3, visitors_sem_name);
  for (int i = 0; i < visitors_cnt; ++i) {
    sem_post(visitor_sem[0]);
  }

  // create rooms shared memory array
  // i - number of days left for i-th room to be occupied
  rooms_shm_fd = createSharedMemory(rooms_shm_name, ROOMS_CNT);

  // create check semaphore
  createSemaphoreSet(check_sem, 1, check_sem_name);

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
  eraseSemaphoreSet(visitor_sem, 3, visitors_sem_name);
  eraseSemaphoreSet(&check_sem, 1, check_sem_name);
  shm_unlink(rooms_shm_name);
  close(rooms_shm_fd);

  printf("Hotel is closed.\n");
  return 0;
}
