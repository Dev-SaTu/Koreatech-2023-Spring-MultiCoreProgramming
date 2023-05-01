#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include "DS_timer.cpp"
#include "DS_definitions.h"

int main(int argc, char **argv)
{
	DS_timer timer(1);
	timer.setTimerName(0, (char *)"Parallel");

	int threads = 4;
	int sum = 0;
	
	timer.onTimer(0);

	#pragma omp parallel shared(sum) num_threads(threads)
	{
		#pragma omp for
		for (int i = 0; i < threads; i++) {
			sum += 1;
			printf("[%d] %d\n", sum);
		}
	}

	timer.offTimer(0);
	timer.printTimer();
	
	return 0;
}