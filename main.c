#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <time.h>

int main()
{
    const int count = 10000000;
    const int random_seed = 92021447;
    const int target = 227;
    const int max_threads = 12;
    int index = -1;
    int found = 0;

    srand(time(NULL));

    int *array = (int *)malloc(count * sizeof(int));
    for (int i = 0; i < count; i++) { array[i] = rand(); }


    for (int threads = 1; threads <= max_threads; threads++)
    {
        found = 0;
        double start_time = omp_get_wtime();

        #pragma omp parallel num_threads(threads) shared(array, count, target, threads, found) reduction(max : index) default(none)
        {
            int length = count/threads;
            
            for (int i = length * omp_get_thread_num(); i < length * (omp_get_thread_num() + 1); i++) {
                if (!found && array[i] == target) {
                    index = i;
                    found = 1;
                    break;
                }
                if (found) {
                    break;
                }
            }
        }

        if (index == -1) {
            printf("Target not found. Threads: %d. Time: %f\n", threads, omp_get_wtime() - start_time);
        } else {
            printf("Found number %d at index %d. Threads: %d. Time: %f\n", target, index, threads, omp_get_wtime() - start_time);
        }
    }
    return (0);
}