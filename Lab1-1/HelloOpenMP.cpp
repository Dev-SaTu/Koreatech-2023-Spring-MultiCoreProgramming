#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

int main(void) {

    #pragma omp parallel num_threads(8)
    {

        int numThreads = omp_get_num_threads(); // 전체 스레드의 수 반환
        int threadNum = omp_get_thread_num(); // 자신의 스레드 ID 반환
        
        printf("[Thread %d/%d] Hello OpenMP!\n", threadNum, numThreads);

    }

    return 0;

}