#ifndef DRIVER_H
#define DRIVER_H

extern unsigned int **_multithreadStack;

int instroNumThreads;

/* selfmade continuous ids */
extern __thread int threadId;
extern volatile int currentThreadNum;
void assingContinuousThreadId();
unsigned long getThreadId();	// for PAPI_init()

void initShadowStack();

void __cyg_profile_func_enter(void *func, void *callsite);
void __cyg_profile_func_exit(void *func, void *callsite);

#endif	// DRIVER_H
