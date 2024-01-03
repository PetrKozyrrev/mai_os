#include <stdio.h>
#include "funcs.h"

int GCF(int A, int B){
    return B ? GCF(B, A % B) : A;
}

float Square(float A,float B){
    return A * B;
}
