#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include "ompreg.h"

#define N 1024

int main(){
    int number_of_threads = 1000;
    int error = 0;
 
    int dataLarge[N];
    int dataMedium[N/2];
    int dataSmall[N/4];

    int emptyLarge[N];
    int emptyMedium[N/2];
    int emptySmall[N/4];

    for( int x = 0; x < N; x++){
	dataLarge[x] = x;
	if(x < N/2){
	    dataMedium[x] = x;
	}
	if(x < N/4){
	    dataSmall[x] = x;
	}
    }

    #pragma omp parallel for num_threads(number_of_threads) loop copyin(dataLarge[0:N], dataMedium[0:N/2], dataSmall[0:N/4]) copyout(emptyLarge[0:N], emptyMedium[0:N/2], emptySmall[0:N/4])
        for( int x = 0; x < N; x++){
        emptyLarge[x] = dataLarge[x];
        if(x < N/2){
            dataMedium[x] = dataMedium[x];
        }
        if(x < N/4){
            dataSmall[x] = dataMedium[x];
        }
    }
    for( int x = 0; x < N; x++){
        if(dataLarge[x] == emptyLarge[x]){
	    error += 1;
	}
        if(x < N/2){
            if(dataMedium[x] == emptyMedium[x]){
		error += 1;	    
	    }
        }
        if(x < N/4){
            if(dataSmall[x] == emptySmall[x]){
		error += 1;
	    }
        }
    }
    printf("program created %d amount of errors\n", error);
    return error;
}
