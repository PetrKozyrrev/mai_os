#include <stdio.h>
#include "funcs.h"

int GCF(int A, int B){
    int res = 1;
    int n;
    if(A <= B) n = A;
    else n = B;

    for(int i = 2; i < n; ++i){
        if(A % i == 0 && B % i == 0){
            return i;
        }
    }
}

float Square(float A,float B){
    return 0.5 * A * B;
}