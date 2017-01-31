#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/syscall.h>
#include <string.h>
#include <pthread.h>

#include <sys/timeb.h>
#include <time.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>

#include "governor_part1.h"

// Workload that traverses allocated memory area in a reverse order (to minimize prefetching effect)
static int DEF_ITERATION = 10000;
static int DEF_STRIDE = 128;
static int DEF_SIZE = 128*1024;

static int* v;
void workload_init(int STRIDE, int SIZE) {
	// Set up array
	v = (int*)malloc(sizeof(int)*SIZE);
	int i;
	for (i = STRIDE; i < SIZE; i += STRIDE) {
		v[i] = (int)&v[i-STRIDE];
	}
	v[0] = (int)&v[i-STRIDE];

	// Warmup the memory
	int* p = v;
	do {
		p = (int*)*p;
	} while (p != (int*)v);
}

void workload_finish() {
	free(v);
}

void workload_body(int iteration, int STRIDE, int SIZE) {
	if (SIZE <= STRIDE) return;

	// Get result
	long long sum_t = 0;
	int n = 0;
	int* p = v;
	int i;

	for (i = 0; i < iteration; i++) {
		p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; 
		p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; 
		p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; 
		p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; 
		p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; 
		p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; 
		p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; 
		p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; 
		p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; 
		p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; 
		p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; 
		p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; 
		p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; 
		p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; 
		p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; 
		p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; 
		p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; 
		p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; 
		p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; 
		p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; 
		p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; 
		p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; 
		p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; 
		p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; 
		p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; 
		p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; 
		p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; 
		p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; 
		p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; 
		p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; 
		p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; 
		p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; 
		p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; 
		p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; 
		p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; 
		p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; 
		p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; 
		p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; 
		p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; 
		p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; 
		p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; 
		p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; 
		p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; 
		p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; 
		p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; 
		p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; 
		p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; 
		p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; 
		p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; 
		p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; 
		p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; 
		p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; 
		p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; 
		p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; 
		p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; 
		p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; 
		p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; 
		p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; 
		p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; 
		p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; 
		p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; 
		p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; 
		p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; 
		p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; 
		p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; 
		p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; 
		p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; 
		p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; 
		p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; 
		p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; 
		p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; 
		p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; 
		p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; 
		p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; 
		p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; 
		p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; 
		p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; 
		p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; 
		p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; 
		p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; 
		p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; 
		p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; 
		p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; 
		p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; 
		p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; 
		p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; 
		p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; 
		p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; 
		p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; 
		p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; 
		p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; 
		p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; 
		p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; 
		p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; 
		p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; 
		p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; 
		p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; 
		p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; 
		p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; 
		p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; 

	}

}

static long long get_current_time_us() {
	struct timeval tv;
	gettimeofday(&tv, NULL);
	long long curTime = (long long)tv.tv_sec * 1000 * 1000 + (long long)tv.tv_usec;
	return curTime;
}

static inline void reset_counters()
{
	// Reset all cycle counter and event counters
	asm volatile ("mcr p15, 0, %0, c9, c12, 0\n\t" :: "r"(0x00000017));  
}

void performance_test(void) {
	unsigned int v1;
    unsigned int v2;
    unsigned int v3;
    unsigned int v4;
    unsigned int v5;
    unsigned int v6;

	asm volatile ("mcr p15, 0, %0, c9, c12, 5\n\t" :: "r"(0x0)); //select first event counter
	asm volatile ("mcr p15, 0, %0, c9, c13, 1\n\t" :: "r"(0x4)); //L1 Data Cache Access
	asm volatile ("mrc p15, 0, %0, c9, c13, 2\n\t" : "=r" (v1));
	printf("counter 1 value %d\n", v1);

	asm volatile ("mcr p15, 0, %0, c9, c12, 5\n\t" :: "r"(0x1)); //select second event counter
	asm volatile ("mcr p15, 0, %0, c9, c13, 1\n\t" :: "r"(0x3)); //L1 Data Cache Miss
	asm volatile ("mrc p15, 0, %0, c9, c13, 2\n\t" : "=r" (v2));
	printf("counter 2 value %d\n", v2);

	asm volatile ("mcr p15, 0, %0, c9, c12, 5\n\t" :: "r"(0x2)); //select third event counter
	asm volatile ("mcr p15, 0, %0, c9, c13, 1\n\t" :: "r"(0x16)); //L2 Data Cache Access
	asm volatile ("mrc p15, 0, %0, c9, c13, 2\n\t" : "=r" (v3));
	printf("counter 3 value %d\n", v3);


	asm volatile ("mcr p15, 0, %0, c9, c12, 5\n\t" :: "r"(0x3)); //select fourth event counter
	asm volatile ("mcr p15, 0, %0, c9, c13, 1\n\t" :: "r"(0x17)); //L2 Data Cache Miss
	asm volatile ("mrc p15, 0, %0, c9, c13, 2\n\t" : "=r" (v4));
	printf("counter 4 value %d\n", v4);

	asm volatile ("mcr p15, 0, %0, c9, c12, 5\n\t" :: "r"(0x4)); //select fifth event counter
	asm volatile ("mcr p15, 0, %0, c9, c13, 1\n\t" :: "r"(0xC9)); //Conditional Branch Executed
	asm volatile ("mrc p15, 0, %0, c9, c13, 2\n\t" : "=r" (v5));
	printf("counter 5 value %d\n", v5);

	asm volatile ("mcr p15, 0, %0, c9, c12, 5\n\t" :: "r"(0x5)); //select sixth event counter
	asm volatile ("mcr p15, 0, %0, c9, c13, 1\n\t" :: "r"(0xCC)); //Conditional Branch mispredicted
	asm volatile ("mrc p15, 0, %0, c9, c13, 2\n\t" : "=r" (v6));
	printf("counter 6 value %d\n", v6);
	printf("finished\n");
}

int main(int argc, char *argv[]) {
	char szOldGovernor[32];
	unsigned long long t0, t1;

	// Initialize
	set_governor("userspace", szOldGovernor);
	set_by_min_freq(); // You can also test the minimum frequency using "set_by_min_freq()"
	workload_init(DEF_STRIDE, DEF_SIZE);
	printf("Measurement start.\n");
	
	// 1. Reset measurement metrics
	t0 = get_current_time_us();
	reset_counters();

	// 2. Run workload
	workload_body(DEF_ITERATION, DEF_STRIDE, DEF_SIZE);

	// 3. Here, you may need to get performance counters.
	performance_test();
	unsigned int v;
	asm volatile ("mrc p15, 0, %0, c9, c13, 0\n\t" : "=r" (v));
	printf("cycle counter is %d\n", v);

	t1 = get_current_time_us();
	// Print results
	int freq = get_cur_freq();
	printf("Exe_time: %lld us at %d\n", t1-t0, freq);

	// Finish the program
	workload_finish();
	set_governor(szOldGovernor, NULL);

	return 0;
}
