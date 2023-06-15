#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include "ompreg.h"

#define N 1024

int main(){
    int number_of_threads = 10000;
    int error = 0;
 
    int dataLarge[N];
    int dataMedium[N/2];
    int dataSmall[N/4];

    int sumReduction = 1;
    int minusReduction = 1;
    int multiplyReduction = 1;

    int testSum = 1;
    int testMinus = 1;
    int testMultiply = 1;

    for( int x = 0; x < N; x++){
	dataLarge[x] = x;
	testSum += dataLarge[x];
	if(x < N/2){
	    dataMedium[x] = x;
	    testMinus -= dataMedium[x];
	}
	if(x < N/4){
	    dataSmall[x] = x;
	    testMultiply *= dataSmall[x];
	}
    }

    #pragma omp parallel for num_threads(number_of_threads) loop copyin(dataLarge[0:N], dataMedium[0:N/2], dataSmall[0:N/4]) reduction(+:sumReduction, -:minusReduction, *:multiplyReduction) copyout( sumReduction, minusReduction, multiplyReduction)
        for( int x = 0; x < N; x++){
        sumReduction +=  dataLarge[x];
        if(x < N/2){
            minusReduction -= dataMedium[x];
        }
        if(x < N/4){
            multiplyReduction *= dataSmall[x];
        }
    }

    if(testSum != sumReduction){
	error += 1;
    }
    if(testMinus != minusReduction){
	error += 1;
    }

    if(testMultiply != multiplyReduction){
	error += 1;
    }
	
    printf("program created %d amount of errors\n", error);
    printf(" testSum = %d. sumReduction = %d. testMinus = %d. minusReduction = %d, testSum = %d, sumReduction = %d\n", testSum, sumReduction, testMinus, minusReduction, testSum, sumReduction);
    return error;
}
