# OS Lab 6
## Varun Kumar


## Q1: Banker's Algorithm

In q1.c, I used Allocation, Max, and Need matrices to simulate safe resource allocation. The program computes Need as Max - Allocation and checks whether a request keeps the system in a safe state. If not, the request is denied.

## Q2: Deadlock Detection

In q2.c, I built a wait-for graph from Allocation and Request matrices and used DFS to detect a cycle. The program shows one safe scenario and one deadlock scenario.

## Q3: Resource Ordering

In q3_bad.c, Process A takes Lock1 then Lock2, while Process B takes Lock2 then Lock1. This creates a circular wait and deadlock. In q3_fixed.c, both processes follow the same order, Lock1 then Lock2, so circular wait is removed and the processes finish normally.

## Q4: Combined Synchronization

In q4.c, I used resource ordering Printer < Scanner < Disk and pipe-based counting semaphores. Five processes request resources in the same order and release them safely, so no deadlock is formed.
