#ifndef TASK_H
#define TASK_H
#include <stdint.h>

#define NUM_SIGNALS 32

#define SIGINT   2 
#define SIGILL   4  
#define SIGFPE   8   
#define SIGKILL  9  
#define SIGTERM  15

typedef void (*sig_handler_t)(int);

typedef struct {
    void* esp; 
    uint8_t id;
    uint8_t state;
    uint8_t is_active;

    uint32_t pending_signals; 
    sig_handler_t signal_handlers[NUM_SIGNALS];
} Task;

extern Task task_list[4];
extern int current_task;

extern unsigned int task2_stack[1024]; 
extern unsigned int task4_stack[2048]; 

void send_signal(int target_task_id, int signum);
void check_signals(int current_task_id, uint32_t* registers_on_stack);

#endif 
