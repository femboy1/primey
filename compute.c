#include <math.h>
#include <stdio.h>
#include "compute.h"

void* computeBounds(void* _ThreadTable) {
    struct ThreadTable tt = *(struct ThreadTable*)(_ThreadTable);


    const ull lower_bound = tt.lower_bound;
    const ull upper_bound = tt.upper_bound;

    ull* primes = tt.primes;

    ull count = 0;


    long long i = floor((long long)(lower_bound / 6.0));

    if (i == 0) {
        i++;
    }

    for (; i<(long long)((upper_bound) / 6.0); i++) {
        long long pp0 = 6 * i - 1;
        long long pp1 = 6 * i + 1;
        bool p0 = 0;
        bool p1 = 0;

        if ((i - 4) % 5 == 0) {
            p1 = 1;
        };

        for (long long k = 1; k<(long long)(sqrt(pp1) / 2.0)+1; k++) {
            if (!p0 && (pp0 % (2*k + 1) == 0)) {
                p0 = 1;
            };

            if (!p1 && (pp1 % (2*k + 1) == 0)) {
                p1 = 1;
            };
        };


        if (!p0) {
            count++;
            primes[count] = pp0;
        };
        if (!p1) {
            count++;
            primes[count] = pp1;
        };
    };

    *tt.count_element = count;
    *tt.finished_element = 1;

    return NULL;
};