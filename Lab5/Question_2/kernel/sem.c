#include "types.h"
#include "param.h"
#include "riscv.h"
#include "spinlock.h"
#include "proc.h"
#include "defs.h"

#define NSEMS 16

struct sem {
  int value;
  int used;
  int next;
  int serving;
};

static struct spinlock lock;
static struct sem sems[NSEMS];

void
seminit(void)
{
  initlock(&lock, "sem");
  memset(sems, 0, sizeof(sems));
}

int
kseminit(int id, int value)
{
  if (id < 0 || id >= NSEMS || value < 0)
    return -1;
  acquire(&lock);
  sems[id].value = value;
  sems[id].used = 1;
  sems[id].next = 0;
  sems[id].serving = 0;
  release(&lock);
  return 0;
}

int
ksemwait(int id)
{
  int ticket;

  if (id < 0 || id >= NSEMS)
    return -1;
  acquire(&lock);
  if (!sems[id].used) {
    release(&lock);
    return -1;
  }
  ticket = sems[id].next++;
  for (;;) {
    if (ticket == sems[id].serving && sems[id].value > 0) {
      sems[id].value--;
      sems[id].serving++;
      wakeup(&sems[id]);
      release(&lock);
      return 0;
    }
    sleep_prepare(&sems[id]);
    release(&lock);
    sleep();
    acquire(&lock);
  }
}

int
ksempost(int id)
{
  if (id < 0 || id >= NSEMS)
    return -1;
  acquire(&lock);
  if (!sems[id].used) {
    release(&lock);
    return -1;
  }
  sems[id].value++;
  wakeup(&sems[id]);
  release(&lock);
  return 0;
}
