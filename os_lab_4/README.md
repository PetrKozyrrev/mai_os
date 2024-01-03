# **Лабораторные работы №4**
## Группа 2 Вариант 10 Козырев Пётр

### Тема: Динамические библиотеки

### **Цель работы**

Целью является приобретение практических навыков в:
 - Создание динамических библиотек
 - Создание программ, которые используют функции динамических библиотек

### **Задание**

Требуется создать динамические библиотеки, которые реализуют определенный функционал. 
Далее использовать данные библиотеки 2-мя способами:
1. Во время компиляции (на этапе «линковки»/linking)
2. Во время исполнения программы. Библиотеки загружаются в память с помощью интерфейса ОС для работы с динамическими библиотеками

В конечном итоге, в лабораторной работе необходимо получить следующие части:
 - Динамические библиотеки, реализующие контракты, которые заданы вариантом;
 - Тестовая программа (программа №1), которая используют одну из библиотек, используя знания полученные на этапе компиляции;
 - Тестовая программа (программа №2), которая загружает библиотеки, используя только их местоположение и контракты.
Провести анализ двух типов использования библиотек.

Пользовательский ввод для обоих программ должен быть организован следующим образом:
 - Если пользователь вводит команду «0», то программа переключает одну реализацию 
контрактов на другую (необходимо только для программы №2). Можно реализовать 
лабораторную работу без данной функции, но максимальная оценка в этом случае будет 
«хорошо»;
 - «1 arg1 arg2 … argN», где после «1» идут аргументы для первой функции, предусмотренной 
контрактами. После ввода команды происходит вызов первой функции, и на экране 
появляется результат её выполнения;
 - «2 arg1 arg2 … argM», где после «2» идут аргументы для второй функции, 
предусмотренной контрактами. После ввода команды происходит вызов второй функции, 
и на экране появляется результат её выполнения.


### Варианты:

#### Функция 1:

Реализация                                                    | Сигнатура             | Реализация 1     | Реализация 2
--------------------------------------------------------------|:---------------------:|:----------------:|--------------------------------------------------------------------------
Подсчёт наибольшего общего делителя для двух натуральных чисел| Int GCF(int A, int B) | Алгоритм Евклида | Наивный алгоритм. Пытаться разделить числа на все числа, что меньше A и B

#### Функция 2:

Реализация                                                    | Сигнатура                      | Реализация 1        | Реализация 2
--------------------------------------------------------------|:------------------------------:|:-------------------:|--------------------------------------------------------------------------
Подсчет площади плоской геометрической фигуры по двум сторонам| Float Square(float A, float B) | Фигура прямоугольник| Наивный алгоритм. Пытаться разделить числа на все числа, что меньше A и B


### **Решение**

func.h
```c
#pragma once

int GCF(int A,int B);

float Square(float A, float B);
```

first_realization.c
```c
#include <stdio.h>
#include "funcs.h"

int GCF(int A, int B){
    return B ? GCF(B, A % B) : A;
}

float Square(float A,float B){
    return A * B;
}
```

second_realization.c
```c
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
```

main_1.c
```c
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
```

main_2.c
```c
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
```

run.sh
```bash
echo -e "Запуск main_1 с одной реализацией функции\n"

gcc -c main_1.c first_realization.c

gcc main_1.o first_realization.o -o res1

./res1

echo -e "\n------------------------------------------------\n"
echo -e "Запуск main_2\n"

gcc -o libFirst.so -shared -fPIC first_realization.c
gcc -o libSecond.so -shared -fPIC second_realization.c
gcc main_2.c -L. -lFirst -lSecond -o res2
export LD_LIBRARY_PATH=.

./res2
```
