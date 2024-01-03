#include <stdio.h>
#include <dlfcn.h>
#include <string.h>
#include <stdlib.h>

int main(){

    printf("0 - Change realization\n");
    printf("1 arg1 arg2 - GCD function\n");
    printf("2 arg1 arg2 - Square function\n");
    printf("3 - exit\n");

    int flag_lib = 1;      // номер текущей библиотеки
    void *library_handler; // прямой указатель на начало динамической библиотеки

    library_handler = dlopen("libFirst.so",RTLD_LAZY);

    if (!library_handler){
        // если ошибка, то вывести ее на экран
        fprintf(stderr,"dlopen() error: %s\n", dlerror());
        exit(1); // в случае ошибки закончить работу программу
    };

    int command;
    printf("Input command: ");
    scanf("%d",&command);

    while(command != 3){
        switch (command){
        case 0:
            
            dlclose(library_handler); // закрываю текущую библиотеку

            if(flag_lib == 1){
                library_handler = dlopen("libSecond.so",RTLD_LAZY);
                flag_lib = 2;
            }
            else{
                library_handler = dlopen("libFirst.so",RTLD_LAZY);
                flag_lib = 1;
            }

            printf("\nREALIZATION CHANGED\n");

            printf("Input command: ");
            scanf("%d",&command);

            break;

        case 1:
            int x,y;

            printf("\nInput args: ");
            scanf("%d %d",&x,&y);
            printf("-----------------\n");

            int (*gcdfunc)(int, int); // указатель на функцию GCF
            char name1[] = "GCF";

            gcdfunc = dlsym(library_handler,name1);

            if (!gcdfunc){
                fprintf(stderr,"dlopen() error: %s\n", dlerror());
                exit(1);
            };
            
            if(flag_lib == 1) printf("\nGCD realization №1\n");
            else printf("\nGCD realization №2\n");

            printf("GCD(%d, %d) = %d\n",x,y,(*gcdfunc)(x, y));
            printf("-----------------\n");

            printf("\nInput command: ");
            scanf("%d",&command);

            break;

        case 2:

            float a,b;

            printf("\nInput args: ");
            scanf("%f %f",&a,&b);
            printf("-----------------\n");

            float (*squarefunc)(float, float); // указатель на функцию Square
            char name2[] = "Square";

            if(flag_lib == 1) printf("\nSquare realization №1\n");
            else printf("\nSquare realization №2\n");

            squarefunc = dlsym(library_handler,name2);

            printf("Square(%.2f, %.2f) = %.2f\n",a,b,(*squarefunc)(a, b));
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
    dlclose(library_handler);
}