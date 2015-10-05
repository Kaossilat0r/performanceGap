#include <err.h>	// for errx
#include <unistd.h>	// for getpid
#include <stdlib.h>	// for malloc

#include "driver.h"
#include "libtiming_tsc/timing.h"

unsigned int **_multithreadStack = 0;

int instroNumThreads = 1;

__thread int threadId;
volatile int currentThreadNum = 0;


unsigned long getThreadId() {
	return (unsigned long) threadId;
}

void assingContinuousThreadId() {

	if (currentThreadNum >= instroNumThreads) {
		errx(1, "Assigned more than %u threads.", instroNumThreads);
	}
	threadId = currentThreadNum++;
}

void initShadowStack() {

	_multithreadStack = (unsigned int **) malloc(instroNumThreads * sizeof(unsigned int *));
	if (_multithreadStack == 0) {
		fprintf(stderr, "Allocation failed in %s in %s\n", __FILE__, __FUNCTION__);
	}
	for (int i = 0; i < instroNumThreads; i++) {
		_multithreadStack[i] = (unsigned int*) malloc(sizeof(unsigned int));
		if (_multithreadStack[i] == NULL) {
			fprintf(stderr, "Could not allocate memory for multithread stack\n");
		}

		*_multithreadStack[i] = 0;
	}

}

void __cyg_profile_func_enter(void *func, void *callsite) {
#ifdef SERIAL_OPT
	*_multithreadStack[0] += 1;
#else
	*_multithreadStack[threadId] += 1;
#endif
}

void __cyg_profile_func_exit(void *func, void *callsite) {
#ifdef SERIAL_OPT
	*_multithreadStack[0] -= 1;
#else
	*_multithreadStack[threadId] -= 1;
#endif
}

void calcPlus() {
#ifdef SERIAL_OPT
	*_multithreadStack[0] -= 1;
#else
	*_multithreadStack[threadId] -= 1;
#endif
}

void calcMinus() {
#ifdef SERIAL_OPT
	*_multithreadStack[0] -= 1;
#else
	*_multithreadStack[threadId] -= 1;
#endif
}

__attribute__((constructor))
void *monitor_init_process(int *argc, char **argv, void *data) {

	printf("#### init sampling driver - Pid is %i #### \n", getpid());

	assingContinuousThreadId();
	initShadowStack();

	initMeasurement();
	printResultsHeader();

	// warmup libtiming
	startMeasurement();
	stopMeasurement();
//	printResults("warmup");
	startMeasurement();
	stopMeasurement();
//	printResults("warmup");
	startMeasurement();
	stopMeasurement();
//	printResults("warmup");

	startMeasurement();


	return NULL;
}

__attribute__((destructor))
void monitor_fini_process(int how, void* data) {

	stopMeasurement();
	printResults("target");
}

