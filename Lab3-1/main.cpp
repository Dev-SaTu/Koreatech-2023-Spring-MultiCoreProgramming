#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include "DS_timer.cpp"
#include "DS_definitions.h"

int main(int argc, char **argv)
{
	const int N = 1024, M = 10;
	const double binSize = 1.0;
	double data[N] = {};
	double bin[M] = {};

#pragma omp parallel for
	for (int i = 0; i < N; i++)
	{
		double rnd = (double)(rand() % 101) / 10.0;
		data[i] = rnd;
	}

	DS_timer timer(4);
	timer.setTimerName(0, (char *)"Serial");
	timer.setTimerName(1, (char *)"Parallel Version 1");
	timer.setTimerName(2, (char *)"Parallel Version 2");
	timer.setTimerName(3, (char *)"Parallel Version 3");

	//** 1. Serial code **//
	timer.onTimer(0);

	for (int i = 0; i < N; i++)
	{
		int m = ((int) (data[i] * M)) / M;
		printf("%.1f %d\n", data[i], m);
		bin[m] += data[i];
	}

	printf("----\n");
	for (int i = 0; i < M; i++) {
		printf("%f\n", bin[i]);
	}

	timer.offTimer(0);

	//** 2. Parallel code Version 1 **//
	timer.onTimer(1);

	timer.offTimer(1);

	//** 3. Parallel code Version 2 **//
	timer.onTimer(2);

	timer.offTimer(2);

	//** 4. Parallel code Version 3 **//
	timer.onTimer(3);

	timer.offTimer(3);

	//** 5. Result checking code **//
	printf("[Serial] area = %f\n", 0.0);

	timer.printTimer();
}