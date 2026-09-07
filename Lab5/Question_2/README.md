## Varun Kumar
# Question 2

The circular buffer is stored in the page returned by `shm_get()`. The child mappings point to the same physical page. Kernel semaphores use `sleep()` and `wakeup()`. Semaphore 0 counts empty slots, semaphore 1 counts full slots, and semaphore 2 is the mutex.

Build and run:

```sh
make qemu
prodcons
```

An optional buffer size from 1 to 5 can be passed, such as `prodcons 3`.
