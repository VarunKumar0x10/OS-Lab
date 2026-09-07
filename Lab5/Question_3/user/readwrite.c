#include "kernel/types.h"
#include "user/user.h"

#define TURN 0
#define RESOURCE 1
#define RMUTEX 2
#define START 3

struct data {
  int value;
  int read_count;
  int active_readers;
  int active_writers;
};

void
reader(struct data *s)
{
  int i;

  sem_wait(START);
  for (i = 0; i < 3; i++) {
    sem_wait(TURN);
    sem_post(TURN);

    sem_wait(RMUTEX);
    s->read_count++;
    if (s->read_count == 1)
      sem_wait(RESOURCE);
    s->active_readers++;
    printf("T%d Reader PID %d entered, readers = %d, data = %d\n",
           uptime(), getpid(), s->active_readers, s->value);
    sem_post(RMUTEX);

    pause(2);

    sem_wait(RMUTEX);
    s->active_readers--;
    printf("T%d Reader PID %d left, readers = %d\n",
           uptime(), getpid(), s->active_readers);
    s->read_count--;
    if (s->read_count == 0)
      sem_post(RESOURCE);
    sem_post(RMUTEX);
    pause(1);
  }
}

void
writer(struct data *s)
{
  int i;

  sem_wait(START);
  for (i = 0; i < 3; i++) {
    sem_wait(TURN);
    sem_wait(RESOURCE);
    s->active_writers = 1;
    s->value++;
    printf("T%d Writer PID %d writing, readers = %d, writers = %d, data = %d\n",
           uptime(), getpid(), s->active_readers, s->active_writers, s->value);
    pause(2);
    s->active_writers = 0;
    printf("T%d Writer PID %d left\n", uptime(), getpid());
    sem_post(RESOURCE);
    sem_post(TURN);
    pause(1);
  }
}

int
main(void)
{
  struct data *s = shm_get();
  int i;
  int pid;

  if (s == (void *)-1)
    exit(1);
  memset(s, 0, sizeof(*s));
  sem_init(TURN, 1);
  sem_init(RESOURCE, 1);
  sem_init(RMUTEX, 1);
  sem_init(START, 0);

  for (i = 0; i < 3; i++) {
    pid = fork();
    if (pid == 0) {
      reader(s);
      exit(0);
    }
    if (pid < 0)
      exit(1);
  }

  for (i = 0; i < 2; i++) {
    pid = fork();
    if (pid == 0) {
      writer(s);
      exit(0);
    }
    if (pid < 0)
      exit(1);
  }

  for (i = 0; i < 5; i++)
    sem_post(START);
  for (i = 0; i < 5; i++)
    wait(0);

  printf("Readers-writers completed, final data = %d\n", s->value);
  exit(s->value == 6 ? 0 : 1);
}
