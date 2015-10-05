
#include "stdio.h"

#ifdef __cplusplus
extern "C" {
#endif

void initMeasurement();

void startMeasurement();
void stopMeasurement();

void printResultsHeader();
void printResults(const char* name);

#ifdef __cplusplus
}
#endif
