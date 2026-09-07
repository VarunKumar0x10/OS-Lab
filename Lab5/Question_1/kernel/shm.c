#include "types.h"
#include "param.h"
#include "riscv.h"
#include "memlayout.h"
#include "spinlock.h"
#include "proc.h"
#include "defs.h"

static struct spinlock lock;
static void *page;

void
shminit(void)
{
  initlock(&lock, "shm");
  page = 0;
}

uint64
shmget(void)
{
  struct proc *p = myproc();
  pte_t *pte;

  acquire(&lock);
  pte = walk(p->pagetable, SHMVA, 0);
  if (pte && (*pte & PTE_V)) {
    release(&lock);
    return SHMVA;
  }

  if (page == 0) {
    page = kalloc();
    if (page == 0) {
      release(&lock);
      return -1;
    }
    memset(page, 0, PGSIZE);
  }

  if (mappages(p->pagetable, SHMVA, PGSIZE, (uint64)page,
               PTE_R | PTE_W | PTE_U) < 0) {
    release(&lock);
    return -1;
  }
  sfence_vma();
  release(&lock);
  return SHMVA;
}
