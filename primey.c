#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
#include <pthread.h>
#include "compute.h"
#include <stdbool.h>

#include <time.h>
#include <signal.h>

double ratio(unsigned long long int z) {
    return log(z) / 3.141 - 0.212;
}


int main(int argc, const char** argv) {
    if (argc < 2) {
        printf("\e[31;1;3mError! Expected max value (got 1 argument, expected 2).\e[0m\n");
        return 1;
    };

    // gather sysinfo

    short int cpus = sysconf(_SC_NPROCESSORS_ONLN);
    float free_ram = sysconf(_SC_PHYS_PAGES) * sysconf(_SC_PAGE_SIZE) / 1073741824.0;


    printf("\e[38;5;245;3m--- system information ---\e[0m\n");
    printf("\u2022 cpus (threads): \e[32;1m%d\e[0m\n", cpus);
    printf("\u2022 free memory: \e[35;1m%.1f GB\e[0m\n", free_ram);

    printf("\n");

    char* file = NULL;
    char* ff = NULL;
    char arg_max = 1;

    char* c_str;
    char* cc = NULL;

    /*
    * Ignore this disgusting mess of an argparse
    */
    for (int i = 1; i<argc; i++) {
        if (argv[i][1] == '-') {
            switch (argv[i][2]) {
                case 'f':
                    file = malloc(strlen(argv[i]));
                    strcpy(file, argv[i]);
                    ff = file; // cache file pointer apparently
                    file = strtok(file, "=");
                    file = strtok(NULL, "=");
                    break;
                case 'j':
                        c_str = malloc(strlen(argv[i]));
                        cc = c_str;
                        strcpy(c_str, argv[i]);
                        c_str = strtok(c_str, "=");
                        c_str = strtok(NULL, "=");
                        if (c_str == NULL) {
                            printf("\e[31;1;3mError! Empty cpus argument!\e[0m\n");
                            free(cc);
                            if (ff != NULL) {
                                free(ff);
                            }
                            return 1;
                        };
                        short int old_cpus = cpus;
                        cpus = atoi(c_str);
                        if (cpus > old_cpus) {
                            printf("\e[31;1;3mError! Not enough cpus!\e[0m\n");
                            free(cc);
                            if (ff != NULL) {
                                free(ff);
                            };
                            return 1;
                        }
                        break;

                default:
                    printf("\e[31;1;3mError! Unexpected argument '%s'!\e[0m\n", argv[i]);
                    return 1;
                    break;
            }
        };
        if (isdigit(argv[i][0])) {
            arg_max = i;
        };
    };

    if (file == NULL) {
        printf("\e[31;1;3mError! Empty file argument!\e[0m\n");
        free(ff);
        return 1;
    };

    char* endPtr;
    unsigned long long int max = strtoull(argv[(int)arg_max], &endPtr, 10);

    printf("\e[38;5;245;3m--- given arguments ---\e[0m\n");
    printf("\u2022 file to write: \e[33m\"%s\"\e[0m\n", file);
    printf("\u2022 maximum: \e[34;3m%lld\e[0m\n", max);
    printf("\u2022 threads to use: \e[34;3m%d\e[0m\n", cpus);

    printf("\n");

    unsigned long long int pmax = (unsigned long long int)(max / 3.0);

    printf("\e[38;5;245;3m--- predictive computations ---\e[0m\n");
    printf("\u2022 predicted (simple): \e[34;3m%lld\e[0m\n", pmax);
    printf("\u2022 predicted (analytic): \e[34;3m%.2f\e[0m\n", pmax / ratio(max));

    printf("\n");

    
    pthread_t* ths = (pthread_t*)(calloc(cpus, sizeof(pthread_t)));

    if (ths == NULL) {
        return -1;
    };

    // Allocate bounds
    
    struct ThreadTable* threadtable = (struct ThreadTable*)malloc((cpus+1)*sizeof(struct ThreadTable));

    if (threadtable == NULL) {
        return -1;
    }

    bool* finished_array = (bool*)calloc(cpus, sizeof(bool));
    long long* count_array = (long long*)calloc(cpus, sizeof(long long));

    if (finished_array == NULL || count_array == NULL) {
        return -1;
    }

    int remainder = max - (pmax * 3);

    printf("\e[38;5;245;3m--- jobs ---\e[0m\n");
    
    for (int i = 0; i<cpus; i++) {
        threadtable[i].lower_bound = (ull)(((double)max / cpus)*i);
        
        if (i == cpus-1) {
            threadtable[i].lower_bound += (ull)remainder;
        };

        threadtable[i].upper_bound = (ull)(((double)max / cpus)*(i+1));

        if (i == cpus-1) {
            threadtable[i].upper_bound += 6;
        }

        printf("\e[33m%d\e[0m ==> lbnd: \e[32m%lld\e[0m, upbnd: \e[32m%lld\e[0m\n", i, threadtable[i].lower_bound, threadtable[i].upper_bound);

        threadtable[i].thread = &ths[i];
        threadtable[i].finished_element = &finished_array[i];
        threadtable[i].count_element = &count_array[i];

        // allocate memory
        threadtable[i].primes = 
            (ull*)malloc(sizeof(ull) * 
                ( (size_t)(threadtable[i].upper_bound - threadtable[i].lower_bound) / 3.0 ) + 1
            );

        if (threadtable[i].primes == NULL) {
            return -1;
        };
    };

    struct timespec start, finish;
    double elapsed;

    for (int i = 0; i<cpus; i++) {
        pthread_create(&ths[i], NULL, computeBounds, &threadtable[i]);
    };

    printf("\n\e[38;5;245;3m--- progress ---\e[0m\n");

    clock_gettime(CLOCK_MONOTONIC, &start);

    bool is_working = 1;
    while (is_working) {
        printf("\e[?25l");
        is_working = 0;
        printf("\e[0m\r[");
        for (int i = 0; i<cpus; i++) {
            if (finished_array[i] == 0) {
                is_working = 1;
                printf("\e[31m#\e[0m");
            } else {
                printf("\e[32m#\e[0m");
            };
        };
        printf("]\e[0m");
        printf("\e[?25h");
        fflush(stdout);
        printf("\e[?25h");
    };
    printf("\e[?25h");
    

    printf("\n\n\e[35;1;3mFinished!\e[0m\n");

    for (int i = 0; i<cpus; i++) {
        pthread_join(ths[i], NULL);
    };

    clock_gettime(CLOCK_MONOTONIC, &finish);

    elapsed = (finish.tv_sec - start.tv_sec);
    elapsed += (finish.tv_nsec - start.tv_nsec) / 1000000000.0;

    printf("\ntime taken: %.2f s\n", elapsed);

    FILE* fp;

    fp = fopen(file, "w+");
    fprintf(fp, "2\n3\n");
    for (int i = 0; i<cpus; i++) {
        for (int j = 1; j<count_array[i]+1; j++) {
            fprintf(fp, "%lld\n", threadtable[i].primes[j]);
        };
        free(threadtable[i].primes);
    };

    fclose(fp);

    free(ths);
    free(threadtable);
    free(ff);
    free(finished_array);
    free(count_array);
    if (cc != NULL) free(cc);

    return 0;
};