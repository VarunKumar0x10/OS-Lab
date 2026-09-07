## Varun Kumar
# Question 3

The shared integer and reader count are stored in a shared page. `RMUTEX` protects the reader count. The first reader locks `RESOURCE` and the last reader unlocks it, so readers can overlap while writers remain exclusive. `TURN` is a turnstile: a waiting writer holds it and prevents new readers from passing, which prevents writer starvation.

Build and run:

```sh
make qemu
readwrite
```

The final shared value must be 6.
