# OS Lab 7

This lab explores four operating-systems ideas by extending xv6-RISC-V: threads
that share memory, semaphore-based synchronization, deadlock analysis, and
priority scheduling with aging. The test programs live in `user/`, and the
kernel changes are in `kernel/`.

This folder contains the Lab 7 files and an xv6 `Makefile`, but it is not a
complete xv6 source tree by itself. To build it, apply these files to the
matching full xv6-RISC-V source tree, keeping the usual `kernel/` and `user/`
layout. The Makefile in the full tree should include the four lab programs in
its `UPROGS` list, as this one does.

## What the lab covers

### Q1 — Threads

`clone()` creates a thread that shares its process's address space, and
`join()` waits for a thread to finish. In `threadtest`, three threads each
increment the same counter 100,000 times. Since the increments are not
protected by a lock, some updates can be lost; the final value can therefore
be lower than 300,000.

### Q2 — Semaphores

The lab adds `sem_init()`, `sem_wait()`, and `sem_signal()` system calls.
`synctest` uses them in two ways:

- A mutex semaphore protects the shared counter while three threads each
  perform 100,000 increments. With the updates protected, the expected total
  is 300,000.
- A producer and consumer share a five-slot circular buffer. Counting
  semaphores track empty and full slots, while a mutex protects access to the
  buffer. The test passes ten items through the buffer.

### Q3 — Deadlock detection and avoidance

`deadlock` runs matrix-based examples of deadlock detection and the Banker's
safety check. It prints resource allocations and outstanding requests, checks
safe and deadlocked states, and evaluates hypothetical resource requests.
It also models a circular wait using two pipe resources and checks that
resource state with the detection algorithm.

### Q4 — Priority scheduling

The scheduler uses numeric priorities from 1 to 20: a smaller number means
higher priority, and a new process starts at priority 10. Processes with the
same priority are scheduled in round-robin order. A runnable process that has
waited for 30 ticks is promoted to reduce starvation.

`priotest` starts four CPU-bound processes at priorities 5, 10, 15, and 20,
reports their completion times, and periodically checks the lowest-priority
process while it runs.

## Build and run

Use Linux or WSL with the tools required by the xv6 version you are using:
`make`, Perl, a RISC-V cross-compiler and binutils, and QEMU's RISC-V system
emulator. The Makefile checks for QEMU 7.2 or newer and uses `bc` for that
version check; building the filesystem image also requires a host C compiler.

1. Start with a complete xv6-RISC-V source tree.
2. Copy the Lab 7 kernel and user files into their matching directories, and
   merge the `UPROGS` entries from this folder's `Makefile` into the full
   tree's Makefile. Keep the original xv6 files in place.
3. From the root of that complete source tree, build and boot xv6:

   ```sh
   make clean
   make qemu
   ```

   If the build cannot find your cross-compiler, set `TOOLPREFIX` to the
   compiler prefix expected by your toolchain.
4. At the xv6 shell prompt, run:

   ```text
   threadtest
   synctest
   deadlock
   priotest
   ```

The output depends on the system and scheduling interleavings. In particular,
do not expect the unsynchronized Q1 counter to have one fixed value. Run the
programs on the integrated source tree before treating any result as verified.

## Screenshots

The repository includes screenshots of the four test programs:

- [Q1 — Thread test](screenshots/q1_threadtest.jpeg)
- [Q2 — Semaphore test](screenshots/q2_synctest.jpeg)
- [Q3 — Deadlock test](screenshots/q3_deadlock.jpg)
- [Q4 — Priority test](screenshots/q4_priotest.jpeg)

## Files at a glance

- `kernel/` — Lab 7 changes to process management, system calls, virtual
  memory, traps, and supporting kernel interfaces.
- `user/` — the four test programs and user-space system call declarations
  and stubs.
- `screenshots/` — captured output for each question.
- `Makefile` — xv6 build configuration with the four tests in `UPROGS`.

