# EPL222 - Operating Systems: Lab Exercises

This repository contains my solutions for the Lab Exercises of the EPL222 Operating Systems course, offered by the Computer Science Department at the University of Cyprus. The course took place during the Spring Semester of 2025/26.

These programs demonstrate core operating system concepts, specifically multithreading, synchronization, monitors, and avoiding concurrency issues like deadlock and starvation.

---

## 📂 Lab Exercise 1

* **Exercise 1: Multithreaded Pi Calculation (`pi_calc.c`)**
    * **Description:** A program that calculates an approximation of the value of Pi. It uses concurrent threads (multithreaded) in C to divide the computational workload.
    * **Concepts:** POSIX Threads (`pthreads`), Mutexes, Workload distribution.

* **Exercise 2: The Monkey Synchronization Problem (`monkey.c`)**
    * **Description:** Models a room with a group of monkeys, one bicycle, and a plate of food. The program models this scenario using semaphores to enforce strict rules: a monkey must eat before cycling, only up to three monkeys can eat at the same time, and only one monkey can use the bicycle at a time.
    * **Concepts:** POSIX Semaphores (`sem_t`), Resource Management, Thread Synchronization.

* **Exercise 3: Traffic Intersection (`traffic.c`)**
    * **Description:** Models a traffic scenario at an intersection of two one-way streets using condition variables. It ensures that only one car crosses at a time and alternates priority between the two roads to prevent prolonged starvation.
    * **Concepts:** POSIX Condition Variables (`pthread_cond_t`), Mutexes, Starvation Prevention.

---

## 📂 Lab Exercise 2

* **Exercise 1: Shop Queue Simulation (`ex1_main.c`, `ex1_monitor.c`, `ex1_monitor.h`)**
    * **Description:** Models a shop where each arriving customer receives a number and joins a queue. The employee serves customers in ascending order of their number. If no customers are present, the employee waits.
    * **Monitor Procedures:**
        * `enter(id)` – models what a customer does: joins the queue and waits until served.
        * `service()` – models what the employee does: dequeues and serves the next customer.
    * **Concepts:** POSIX Threads (`pthreads`), Monitors, Mutexes, Condition Variables, Queue (array-based).

* **Exercise 2: Five-Plates Dining Simulation (ex2_main.c, ex2_monitor.c, ex2_monitor.h)**

    * **Description:** Models a scenario with 5 shared plates and a number of people. Each person flips a coin: heads means they must fill an empty plate with food (waiting if all 5 are full); tails means they take a filled plate and eat (waiting if all plates are empty).
    * **Monitor Procedures:**
        * `work(id)` – models filling a plate with food.
        * `eat(id)` – models emptying a plate (eating).
    * **Concepts:** POSIX Threads (`pthreads`), Monitors, Mutexes, Condition Variables, Resource Management.

---

## ⚙️ Compilation & Execution

All programs require the POSIX threads library. Use the `-lpthread` flag when compiling with `gcc`.

```bash
# Lab Exercise 1
gcc -Wall -o pi_calc pi_calc.c -lpthread
gcc -Wall -o monkey monkey.c -lpthread
gcc -Wall -o traffic traffic.c -lpthread

# Lab Exercise 2
gcc -Wall -o exercise1 ex1_main.c ex1_monitor.c -lpthread
gcc -Wall -o exercise2 ex2_main.c ex2_monitor.c -lpthread
```