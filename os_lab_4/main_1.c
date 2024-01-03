#include <stdio.h>
#include <stdlib.h>
#include "funcs.h"

int main(){
    
    printf("1 arg1 arg2 - GCD function\n");
    printf("2 arg1 arg2 - Square function\n");
    printf("3 - exit\n");

    int command;

    printf("Input command: ");
    scanf("%d",&command);

    while(command != 3){
        switch (command){
        case 1:

            int x,y;

            printf("\nInput args: ");
            scanf("%d %d",&x,&y);
            printf("-----------------\n");

            printf("GCD(%d, %d) = %d\n",x,y,GCF(x,y));
            printf("-----------------\n");

            printf("\nInput command: ");
            scanf("%d",&command);

            break;
        case 2:

            float a,b;

            printf("\nInput args: ");
            scanf("%f %f",&a,&b);
            printf("-----------------\n");

            printf("Square(%.2f, %.2f) = %.2f\n",a,b,Square(a,b));
            printf("-----------------\n");

            printf("\nInput command: ");
            scanf("%d",&command);

            break;

        case 3:
            break;

        default:
            printf("\nIncorrect Input\n");
            exit(1);
            break;
        }
    }
}