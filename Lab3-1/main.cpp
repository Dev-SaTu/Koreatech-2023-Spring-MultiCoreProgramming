#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include "DS_timer.cpp"
#include "DS_definitions.h"

int main(int argc, char **argv)
{
	const int N = 1024 * 1024 * 1024, M = 60;
	float* data = (float*)malloc(N * sizeof(float));
	int bin[4][M] = {0};

	const int NUM_THREADS = 4;

	#pragma omp parallel for num_threads(NUM_THREADS)
	for (int i = 0; i < N; i++)
	{
		float rnd = (float)(rand() % 100) / 10.0;
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
		bin[0][(int) (data[i] / 10.0 * M)] += 1;
	}

	timer.offTimer(0);

	//** 2. Parallel code Version 1 **//
	timer.onTimer(1);

	omp_lock_t lock;
	omp_init_lock(&lock);

	/*#pragma omp parallel for num_threads(NUM_THREADS)
	for (int i = 0; i < N; i++)
	{
		omp_set_lock(&lock);
		bin[1][(int) (data[i] / 10.0 * M)] += 1;
		omp_unset_lock(&lock);
	}*/

	timer.offTimer(1);

	//** 3. Parallel code Version 2 **//
	timer.onTimer(2);

	#pragma omp parallel num_threads(NUM_THREADS)
	{
		int local_bin[M] = {0};

		#pragma omp for nowait
		for (int i = 0; i < N; i++)
		{
			local_bin[(int) (data[i] / 10.0 * M)] += 1;
		}

		omp_set_lock(&lock);
		for (int i = 0; i < M; i++)
		{
			bin[2][i] += local_bin[i];
		}
		omp_unset_lock(&lock);
	}

	omp_destroy_lock(&lock);

	timer.offTimer(2);

	//** 4. Parallel code Version 3 **//
	timer.onTimer(3);

	#pragma omp parallel num_threads(NUM_THREADS)
	{
		int local_bin[M] = {0};

		#pragma omp for
		for (int i = 0; i < N; i++)
		{
			local_bin[(int) (data[i] / 10.0 * M)] += 1;
		}

		for (int j = 0; j < M; j++)
		{
			bin[3][j] += local_bin[j];
		}
	}

	timer.offTimer(3);

	//** 5. Result checking code **//
	bool b1 = true, b2 = true, b3 = true;

	for (int i = 0; i < M; i++)
	{
		if (bin[0][i] != bin[1][i])
		{
			b1 = false;
		}
		if (bin[0][i] != bin[2][i])
		{
			b2 = false;
		}
		if (bin[0][i] != bin[3][i])
		{
			b3 = false;
		}
	}

	printf("(N: %d, M: %d) Check: %d, %d, %d\n", N, M, b1, b2, b3);
	
	timer.printTimer();

	free(data);
}