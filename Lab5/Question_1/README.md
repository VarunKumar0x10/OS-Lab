## Varun Kumar
# Question 1

`shm_get()` maps one kernel-allocated page at the same virtual address in a process. `fork()` maps the same physical page in the child. Peterson's `flag` and `turn` values are stored in this page. Memory barriers keep their accesses ordered on RISC-V.

Build and run:

```sh
make qemu
peterson
```

The final counter must be 20.
