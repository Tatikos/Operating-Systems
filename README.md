# EPL222 - Operating Systems: Lab Exercise 1

[cite_start]This repository contains my solutions for the 1st Lab Exercise of the EPL222 Operating Systems course [cite: 2, 4][cite_start], offered by the Computer Science Department at the University of Cyprus[cite: 1]. [cite_start]The course took place during the Spring Semester of 2025/26[cite: 2].

These three C programs demonstrate core operating system concepts, specifically multithreading, synchronization, and avoiding concurrency issues like deadlock and starvation.

## 📂 Project Overview

* **Exercise 1: Multithreaded Pi Calculation (`ex1.c`)**
    * [cite_start]**Description:** A program that calculates an approximation of the value of Pi[cite: 6]. [cite_start]It uses concurrent threads (multithreaded) in C to divide the computational workload[cite: 13].
    * **Concepts:** POSIX Threads (`pthreads`), Mutexes, Workload distribution.

* **Exercise 2: The Monkey Synchronization Problem (`ex2.c`)**
    * [cite_start]**Description:** Models a room with a group of monkeys, one bicycle, and a plate of food[cite: 16]. [cite_start]The program models this scenario using semaphores [cite: 19] [cite_start]to enforce strict rules: a monkey must eat before cycling, only up to three monkeys can eat at the same time, and only one monkey can use the bicycle at a time[cite: 17, 18].
    * **Concepts:** POSIX Semaphores (`sem_t`), Resource Management, Thread Synchronization.

* **Exercise 3: Traffic Intersection (`traffic.c`)**
    * [cite_start]**Description:** Models a traffic scenario at an intersection of two one-way streets [cite: 23] [cite_start]using condition variables[cite: 23]. [cite_start]It ensures that only one car crosses at a time [cite: 26] [cite_start]and alternates priority between the two roads to prevent prolonged starvation[cite: 28].
    * **Concepts:** POSIX Condition Variables (`pthread_cond_t`), Mutexes, Starvation Prevention.

## ⚙️ Compilation & Execution

All programs require the POSIX threads library. Use the `-pthread` flag when compiling with `gcc`.