#ifndef EXERCISE1_MONITOR_H
#define EXERCISE1_MONITOR_H

#define MAX_CUSTOMERS 100


void monitor_init(void);
void monitor_destroy(void);
int  empty(void);
void enQueue(int i);
int  deQueue(void);
void enter(int id); 
void service(void);

#endif