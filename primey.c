#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>

int main(int argc, const char** argv) {
    if (argc < 2) {
        printf("\e[31;1;3mError! Expected max value (got 1 argument, expected 2).\e[0m\n");
        return 1;
    };

    // gather sysinfo

    short int cpus = sysconf(_SC_NPROCESSORS_ONLN);
    float free_ram = sysconf(_SC_PHYS_PAGES) * sysconf(_SC_PAGE_SIZE) / 1073741824.0;


    printf("\e[38;5;245;3m--- system information ---\e[0m\n");
    printf("\u2022 cpus (threads): %d\n", cpus);
    printf("\u2022 free memory: %.1f GB\n", free_ram);

    printf("\n");

    printf("\e[38;5;245;3m--- given arguments ---\e[0m\n");

    return 0;
};