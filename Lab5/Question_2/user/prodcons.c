#include "kernel/types.h"
#include "user/user.h"

#define MAX 5
#define EMPTY 0
#define FULL 1
#define MUTEX 2

struct data {
  int buf[MAX];
  int in;
  int out;
  volatile int count;
};

void
producer(struct data *s, int size)
{
  int i;

  for (i = 1; i <= 20; i++) {
    if (s->count == size)
      printf("Producer blocked: buffer full\n");
    sem_wait(EMPTY);
    sem_wait(MUTEX);
    s->buf[s->in] = i;
    s->in = (s->in + 1) % size;
    s->count++;
    printf("Produced %d, count = %d\n", i, s->count);
    sem_post(MUTEX);
    sem_post(FULL);
    pause(1);
  }
}

void
consumer(struct data *s, int size)
{
  int i;
  int item;

  for (i = 1; i <= 20; i++) {
    if (s->count == 0)
      printf("Consumer blocked: buffer empty\n");
    sem_wait(FULL);
    sem_wait(MUTEX);
    item = s->buf[s->out];
    s->out = (s->out + 1) % size;
    s->count--;
    printf("Consumed %d, count = %d\n", item, s->count);
    if (item != i) {
      printf("Order error: expected %d\n", i);
      exit(1);
    }
    sem_post(MUTEX);
    sem_post(EMPTY);
    pause(3);
  }
  printf("Consumer received all 20 items in order\n");
}

int
main(int argc, char **argv)
{
  struct data *s = shm_get();
  int size = MAX;
  int pid;

  if (argc > 1)
    size = atoi(argv[1]);
  if (size < 1 || size > MAX || s == (void *)-1) {
    printf("invalid buffer size\n");
    exit(1);
  }

  memset(s, 0, sizeof(*s));
  sem_init(EMPTY, size);
  sem_init(FULL, 0);
  sem_init(MUTEX, 1);

  pid = fork();
  if (pid == 0) {
    consumer(s, size);
    exit(0);
  }
  if (pid < 0)
    exit(1);

  pause(5);
  pid = fork();
  if (pid == 0) {
    producer(s, size);
    exit(0);
  }
  if (pid < 0)
    exit(1);

  wait(0);
  wait(0);
  printf("Producer-consumer test completed\n");
  exit(0);
}
