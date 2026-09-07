#include "kernel/types.h"
#include "user/user.h"

struct data {
  volatile int flag[2];
  volatile int turn;
  volatile int counter;
};

void
delay(void)
{
  volatile int i;
  for (i = 0; i < 200000; i++)
    ;
}

void
run(struct data *s, int id)
{
  int i;
  int other = 1 - id;

  for (i = 0; i < 10; i++) {
    s->flag[id] = 1;
    __sync_synchronize();
    s->turn = other;
    __sync_synchronize();

    while (s->flag[other] && s->turn == other)
      ;

    s->counter++;
    printf("Process %d in CS, counter = %d\n", id, s->counter);
    printf("Process %d exits CS\n", id);

    __sync_synchronize();
    s->flag[id] = 0;
    delay();
  }
}

int
main(void)
{
  struct data *s = shm_get();
  int pid;

  if (s == (void *)-1) {
    printf("shm_get failed\n");
    exit(1);
  }

  s->flag[0] = 0;
  s->flag[1] = 0;
  s->turn = 0;
  s->counter = 0;

  pid = fork();
  if (pid < 0) {
    printf("fork failed\n");
    exit(1);
  }

  if (pid == 0) {
    run(s, 1);
    exit(0);
  }

  run(s, 0);
  wait(0);
  printf("Final counter = %d\n", s->counter);
  exit(s->counter == 20 ? 0 : 1);
}
