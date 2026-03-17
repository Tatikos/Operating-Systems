#ifndef EXERCISE2_MONITOR_H
#define EXERCISE2_MONITOR_H

#define NUM_PLATES 5


void monitor_init(int num_people);
void monitor_destroy(void);
void thread_done(void);
void work(int id);
void eat(int id);    

#endif