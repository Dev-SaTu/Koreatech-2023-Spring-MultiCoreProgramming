#include <stdio.h>
#include <time.h>
#include <iostream>
#include <random>
#include <cmath>
#include <omp.h>
using namespace std;

#define M 512
#define N 2048
#define K 1024

int main(void) {
    
    // Memory Allocation
    float *hostA = (float*)malloc(sizeof(float) * M * K);
    float *hostB = (float*)malloc(sizeof(float) * K * N);
    float *hostC = (float*)malloc(sizeof(float) * M * N);

    // Data Generation
    random_device rd;
    mt19937 gen(rd());
    uniform_real_distribution<float> dis(0.0, 1.0);

    for (int i = 0; i < M; i++) {
        for (int j = 0; j < K; j++) {
            hostA[i * K + j] = dis(gen);
        }
    }

    for (int i = 0; i < K; i++) {
        for (int j = 0; j < N; j++) {
            hostB[i * N + j] = dis(gen);
        }
    }

    // None
    clock_t start = clock();
    for (int i = 0; i < M; i++) {
        for (int j = 0; j < N; j++) {
            float sum = 0;
            for (int k = 0; k < K; k++) {
                sum += hostA[i * K + k] * hostB[k * N + j];
            }
            hostC[i * N + j] = sum;
        }
    }
    printf("CPU: %fs\n", (double)(clock() - start) / CLOCKS_PER_SEC);

    // OpenMP
    start = clock();
    #pragma omp parallel for
    for (int i = 0; i < M; i++) {
        for (int j = 0; j < N; j++) {
            float sum = 0;
            for (int k = 0; k < K; k++) {
                sum += hostA[i * K + k] * hostB[k * N + j];
            }
            hostC[i * N + j] = sum;
        }
    }
    printf("OpenMP: %fs\n", (double)(clock() - start) / CLOCKS_PER_SEC);
    
    return 0;
}