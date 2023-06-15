#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include "ompreg.h"

#define N 32768

int main(){
    int number_of_threads = 1000;
    int error = 0;
    int temp = (N*.875);
 
    int data8[N];
    int empty8[N];

    int data7[temp];
    int empty7[temp];

    temp = (N*.750);
    int data6[temp];
    int empty6[temp];

    temp = (N*.635);
    int data5[temp];
    int empty5[temp];

    temp = (N*.500);
    int data4[temp];
    int empty4[temp];

    temp = (N*.375);
    int data3[temp];
    int empty3[temp];

    temp = (N*.205);
    int data2[temp];
    int empty2[temp];

    temp = (N*.125);
    int empty1[temp];
    int data1[temp];

    for( int x = 0; x < N; x++){
	data8[x] = x;
	if(x < N*.875){
	    data7[x] = x;
	}
	if(x < N*.750){
	    data6[x] = x;
	}
        if(x < N*.635){
	    data5[x] = x;
	}
        if(x < N*.500){
	    data4[x] = x;
	}
        if(x < N*.375){
	    data3[x] = x;
	}
        if(x < N*.250){
	    data2[x] = x;
	}
        if(x < N*.125){
	    data1[x] = x;
	}
    }

    #pragma omp parallel for num_threads(number_of_threads) loop copyin(data8[0:N], data7[0:N*.875], data6[0:N*.750], data5[0:N*.635], data4[0:N*.500], data3[0:N*.375], data2[0:N*.250], data1[0:N*.125]) copyout(empty8[0:N], empty7[0:N*.875], empty6[0:N*.750], empty5[0:N*.635], empty4[0:N*.500], empty3[0:N*.375], empty2[0:N*.250], empty1[0:N*.125])
    for( int x = 0; x < N; x++){
        data8[x] = empty8[x];
        if(x < N*.875){
            empty7[x] = data7[x];
        }
        if(x < N*.750){
            empty6[x] = data6[x];
        }
        if(x < N*.635){
            empty5[x] = data5[x];
        }
        if(x < N*.500){
            empty4[x] = data4[x];
        }
        if(x < N*.375){
            empty3[x] = data3[x];
        }
        if(x < N*.250){
            empty2[x] = data2[x];
        }
        if(x < N*.125){
            empty1[x] = data1[x];
        }
    }

    for( int x = 0; x < N; x++){
        if(data8[x] != empty8[x]){
	    error += 1;
	}
        if(x < N*.875){
	    if(data7[x] != empty7[x]){
		error += 1;
	    }
        }
        if(x < N*.750){
            if(data6[x] != empty6[x]){
		error += 1;
	    }
        }
        if(x < N*.635){
            if(data5[x] != empty5[x]){
		error += 1;
	    }
        }
        if(x < N*.500){
            if(data4[x] != empty4[x]){
		error += 1;
	    }
        }
        if(x < N*.375){
            if(data3[x] != empty3[x]){
		error += 1;
	    }
        }
        if(x < N*.250){
            if(data2[x] != empty2[x]){
		error += 1;
	    }
        }
        if(x < N*.125){
            if(data1[x] != empty1[x]){
		error += 1;
	    }
        }
    }

    printf("program created %d amount of errors\n", error);
    return error;
}
