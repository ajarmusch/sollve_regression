//===--- test_target_update_depend.c ----------------------------------------===//
//
// OpenMP API Version 4.5 Jun 13 2023
//
//
//
//
// Author: Aaron Jarmusch <jarmusch@udel.edu>
////===----------------------------------------------------------------------===//

#include <stdio.h>
#include "omp.h"
#include "ompreg.h"

#define N 1024

void kernel(int id)
{
    // Perform computations for the kernel
    printf("Kernel %d executed by thread %d\n", id, omp_get_thread_num());
}

int test_omp_thread_kernel() {

    int errors = 0;

    int num_threads;

    #pragma omp parallel
    {
        #pragma omp single
        {
            num_threads = omp_get_num_threads();
            printf("Number of threads: %d\n", num_threads);            
        }

        #pragma omp for
        for (int i = 0; i < N; i++)
        {
            #pragma omp task
            {
                kernel(i);
            }
        }
    }
    return errors;
}

int main(void)
{
    OMPVV_TEST_OFFLOADING;
    int errors = 0;

    OMPVV_TEST_AND_SET_VERBOSE(errors, test_omp_thread_kernel() != 0);

    // No error will be reported even if it is recorded.
    OMPVV_REPORT_AND_RETURN(errors);
}