#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include "DS_timer.cpp"
#include "DS_definitions.h"

#define SQUARE(X) (X) * (X)

int main(int argc, char** argv)
{

	double a = atoi(argv[1]), b = atoi(argv[2]);
	int n = atoi(argv[3]);

	DS_timer timer(2);
	timer.setTimerName(0, (char*)"Serial");
	timer.setTimerName(1, (char*)"Parallel");

	double h = (b - a) / n;
	double areaSerial = 0.0, areaParallel = 0.0;

	//** 1. Serial code **//
	timer.onTimer(0);

	for (int i = 1; i < n; i++) {
		double y1 = SQUARE(a + (i - 1) * h);
		double y2 = SQUARE(a + i * h);
		areaSerial += ((y1 + y2) / 2.0) * h;
	}

	timer.offTimer(0);

	//** 2. Parallel code **//
	timer.onTimer(1);
	
	const int tNum = 8;
	double pAreaParallel[tNum];

	#pragma omp parallel num_threads(tNum)
	{
		#pragma omp for
		for (int i = 1; i < n; i++) {
			double y1 = SQUARE(a + (i - 1) * h);
			double y2 = SQUARE(a + i * h);
			pAreaParallel[omp_get_thread_num()] += ((y1 + y2) / 2.0) * h;	
		}
		areaParallel += pAreaParallel[omp_get_thread_num()];
	}

	timer.offTimer(1);

	//** 3. Result checking code **//
	printf("f(x) = x * x\n");
	printf("range = (%f, %f), n = %d\n", a, b, n);
	printf("[Serial] area = %f\n", areaSerial);
	printf("[Parallel] area = %f\n", areaParallel);

	timer.printTimer();
}