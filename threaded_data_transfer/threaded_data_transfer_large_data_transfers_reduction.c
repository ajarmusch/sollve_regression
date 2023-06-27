#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include "ompreg.h"

#define N 32768

int main(){
    int number_of_threads = 1000;
    int error = 0;
 
    int data6[N];
    int test6 = 1;
    int device6 = 1;

    int temp = (N*.825);
    int data5[temp];
    int test5 = 1;
    int device5 = 1;

    temp = (N*.660);
    int data4[temp];
    int test4 = 1;
    int device4 = 1;

    temp = (N*.495);
    int data3[temp];
    int test3 = 1;
    int device3 = 1;

    temp = (N*.330);
    int data2[temp];
    int test2 = 1;
    int device2 = 1;

    temp = (N*.165);
    int data1[temp];
    int test1 = 1;
    int device1 = 1;

    for( int x = 0; x < N; x++){
	data6[x] = x;
	test6 += x;
	if(x < N*.825){
	    data5[x] = x;
	    test5 -= x;
	}
	if(x < 20){
	    data4[x] = x;
	    if(x != 0){
		test4 *= x;
	    }
	}
        if(x < N*.495){
	    data3[x] = x;
	    test3 &= x;
	}
        if(x < N*.330){
	    data2[x] = x;
	    test2 |= x;
	}
        if(x < N*.165){
	    data1[x] = x;
	    test1 ^= x;
	}
    }

    #pragma omp parallel for num_threads(number_of_threads) loop copyin(data8[0:N], data7[0:N*.875], data6[0:N*.750], data5[0:N*.635], data4[0:N*.500], data3[0:N*.375], data2[0:N*.250], data1[0:N*.125]) reduction(+:device6, -:device5, *:device4, &: device3, |:device2, ^:device1) copyout(device6, device5, device4, device3, device2, device1)
    for( int x = 0; x < N; x++){
        device6  += data6[x];
        if(x < N*.825){
           device5 -= data5[x];
        }
        if(x < 20){
	    if(x != 0){
            	device4 *= data4[x];
	    }
        }
        if(x < N*.495){
            device3 &= data3[x];
        }
        if(x < N*.330){
            device2 |= data2[x];
        }
        if(x < N*.165){
            device1 ^= data1[x];
        }
    }
    
    if(device6 != test6){
	error += 1;
    }
    if(device5 != test5){
        error += 1;
    }
    if(device4 != test4){
        error += 1;
    }
    if(device3 != test3){
        error += 1;
    }
    if(device2 != test2){
        error += 1;
    }
    if(device1 != test1){
        error += 1;
   }



    printf("program created %d amount of errors\n", error);
    printf("test1 = %d device1 = %d, test2 = %d device2 = %d, test3 = %d device3 = %d, test4 = %d device4 = %d, test5 = %d device5 = %d, test6 = %d device6 = %d\n", test1, device1, test2, device2, test3, device3, test4, device4, test5, device5, test6, device6);
    return error;
}
