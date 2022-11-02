#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
#include <pthread.h>

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




    free(ff);
    if (cc != NULL) free(cc);

    return 0;
};