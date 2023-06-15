//===--- test_target_update_depend.c ----------------------------------------===//
//
// OpenMP API Version 4.5
//
// Create N number of threads and simultaensouly launch
// various kernels. Ensure all kernels are executed correctly.
//
//
// Author: Aaron Jarmusch <jarmusch@udel.edu> Jun 2023
////===----------------------------------------------------------------------===//

#include <stdio.h>
#include "omp.h"
#include "ompreg.h"

#define N 1024 // NUmber of threads
#define NUM_KERNELS 8 // Number of Kernels to launch

int kernel(int a, int b)
{
    // Perform computations for the kernel
    total = a[x] + b[x];

    printf("Kernel %d executed by thread %d\n", i, omp_get_thread_num());

    return total;
}

int test_omp_thread_kernel(void)
{
    int a[N];
    int b[N];
    int total = 0;
    int expect_total = 0;
    int errors = 0;
    int num_threads[N];

    for (int x = 0; x < N; ++x) {
        a[x] = 1;
        b[x] = x;
        num_threads[x] = -1;
    }

    // set the number of threads
    omp_set_num_threads(N);

    #pragma omp parallel
    {
        #pragma omp for reduction(+:total)
        {
            for (int x = 0; x < NUM_KERNELS; ++x) 
            {
                total += kernel(a[x], b[x]);
            }
        }
        #pragma omp for
        {
            for (int x = 0; x < N; ++x) {
                num_threads[x] = omp_get_num_threads();
            }
        }
    }

    #pragma omp barrier 


    for (int x = 0; x < N; ++x) {
        expect_total += a[x] + b[x];
    }

    for (int x = 1; x < N; ++x) {
        OMPVV_WARNING_IF(num_threads[x - 1] != num_threads[x], "Test reported differing numbers of threads.  Validity of testing of reduction clause cannot be guaranteed.");
    }
    OMPVV_WARNING_IF(num_threads[0] == 1, "Test operated with one thread.  Reduction clause cannot be tested.");
    OMPVV_WARNING_IF(num_threads[0] <= 0, "Test reported invalid number of threads.  Validity of testing of reduction clause cannot be guaranteed.");

    OMPVV_TEST_AND_SET_VERBOSE(errors, expect_total != total);
    OMPVV_ERROR_IF(expect_total != total, "Total from loop directive is %d but expected total is %d.", total, expect_total);

    return errors;
}

int main(void)
{
    OMPVV_TEST_OFFLOADING;
    int errors = 0;
    double start_time = omp_get_wtime();

    OMPVV_TEST_AND_SET_VERBOSE(errors, test_omp_thread_kernel() != 0);

    double end_time = omp_get_wtime();

    printf("Execution time: %f seconds\n", end_time - start_time);

    // No error will be reported even if it is recorded.
    OMPVV_REPORT_AND_RETURN(errors);
}