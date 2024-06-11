#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/time.h>

#define NUM_THREADS 3
#define NUM_NUMBERS 1000000

void* threadFunction(void* threadID) {
    int tid = *(int*)threadID;
    char filename[20];
    sprintf(filename, "Data_Thread_%c.txt", 'A' + tid);
    
    FILE* inputFile = fopen("Data.txt", "r");
    FILE* outputFile = fopen(filename, "w");
    
    int number;
    double sum = 0;
    int count = 0;
    
    struct timeval start, end;
    gettimeofday(&start, NULL);
    
    while (fscanf(inputFile, "%d", &number) != EOF) {
        fprintf(outputFile, "%d\n", number);
        sum += number;
        count++;
    }
    
    gettimeofday(&end, NULL);
    double time_taken = (end.tv_sec - start.tv_sec) * 1e3;
    time_taken += (end.tv_usec - start.tv_usec) / 1e3;
    
    double average = sum / count;
    printf("Thread %c: Average = %.2f, Time taken = %.2f ms\n", 'A' + tid, average, time_taken);
    
    fclose(inputFile);
    fclose(outputFile);
    
    pthread_exit(NULL);
}

int main() {
    srand(time(NULL));
    
    FILE* dataFile = fopen("Data.txt", "w");
    for (int i = 0; i < NUM_NUMBERS; i++) {
        fprintf(dataFile, "%d\n", rand() % 101);
    }
    fclose(dataFile);
    
    pthread_t threads[NUM_THREADS];
    int threadIDs[NUM_THREADS];
    
    struct timeval start, end;
    gettimeofday(&start, NULL);
    
    for (int i = 0; i < NUM_THREADS; i++) {
        threadIDs[i] = i;
        pthread_create(&threads[i], NULL, threadFunction, (void*)&threadIDs[i]);
    }
    
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }
    
    gettimeofday(&end, NULL);
    double time_taken = (end.tv_sec - start.tv_sec) * 1e3; 
    time_taken += (end.tv_usec - start.tv_usec) / 1e3;
    
    printf("Main thread: Total time taken = %.2f ms\n", time_taken);
    
    return 0;
}