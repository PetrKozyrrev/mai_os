# **Лабараторная работа №1**
## Группа 2 Вариант 10 Козырев Пётр

### **Цель работы**

Приобретение практических навыков в:
- Управление процессами в ОС
- Обеспечение обмена данных между процессами посредством каналов

### **Задание**

Составить и отладить программу на языке Си, осуществляющую работу с процессами и
взаимодействие между ними в одной из двух операционных систем. В результате работы
программа (основной процесс) должен создать для решение задачи один или несколько
дочерних процессов. Взаимодействие между процессами осуществляется через системные
сигналы/события и/или каналы (pipe).
Необходимо обрабатывать системные ошибки, которые могут возникнуть в результате работы.

Родительский процесс создает дочерний процесс. Первой строчкой пользователь в консоль
родительского процесса вводит имя файла, которое будет использовано для открытия файла с
таким именем на чтение. Стандартный поток ввода дочернего процесса переопределяется
открытым файлом. Дочерний процесс читает команды из стандартного потока ввода.
Стандартный поток вывода дочернего процесса перенаправляется в pipe1. Родительский процесс
читает из pipe1 и прочитанное выводит в свой стандартный поток вывода. Родительский и
дочерний процесс должны быть представлены разными программами.

10 вариант) В файле записаны команды вида: «число<endline>». Дочерний процесс производит
проверку этого числа на простоту. Если число составное, то дочерний процесс пишет это число в
стандартный поток вывода. Если число отрицательное или простое, то тогда дочерний и
родительский процессы завершаются. Количество чисел может быть произвольным.

### **Решение**

main.cpp
```
#include <iostream>
#include <unistd.h>
#include <string>
#include <fstream>
#include <fcntl.h>

int main() {

    const char* CHILD_PATH = "/home/petrkozyrev/osi/os_lab_1/build/child";
    std::string name ="/home/petrkozyrev/osi/os_lab_1/test/";
    
    char smb;
    int ind = 0;

    if( read(STDIN_FILENO, &smb, 1) == -1){
        perror("Reading error\n");
        exit(1);
    }

    while( smb != '\n'){
        name += smb;
        if ( read(STDIN_FILENO, &smb, 1) == -1){
            perror("Reading error\n");
            exit(1);
        }
    }

    const char* file_name = name.c_str();
    
    int pipe_fd[2];
    int err = pipe(pipe_fd);

    if( err == -1){
        perror("Pipe error\n");
        exit(1);
    }
    
    pid_t pid = fork();

    if( pid == -1){
        perror("Pid errror\n");
        exit(1);
    }
    if( pid == 0){ 
        close(pipe_fd[0]);
        int input_file = open(file_name,O_RDONLY);

        if( input_file == -1){
            perror("Cant open file\n");
            exit(1);
        }

        dup2(input_file,STDIN_FILENO);
        
        if( dup2(pipe_fd[1],STDOUT_FILENO) == -1){
            perror("Cant dup2\n");
            exit(1);
        }
        execlp(CHILD_PATH, CHILD_PATH, NULL);
        close(input_file);
        perror("execlp\n");
        exit(EXIT_SUCCESS);
    }
    else{
        close(pipe_fd[1]);

        char buff;
        while( (read(pipe_fd[0], &buff, 1) > 0)){
            write(STDOUT_FILENO,&buff,1);
        }   
        write(STDOUT_FILENO,"\n",1);
        close(pipe_fd[0]);
        exit(EXIT_SUCCESS);
    }
    return 0;
}
```

child_process.cpp

```
#include <iostream>
#include <unistd.h>
#include <string>
#include <fstream>
#include <fcntl.h>
#include "prime.cpp"

int main(int argc, char* argv[]){

    char buff[256];
    char smb;
    int number=0,sgn = 1,ind = 0,len_lst_number = 0;
    
    if (read(STDIN_FILENO, &smb, 1) == -1){
        return -1;
    }
    while( smb != ' '){
        buff[ind] = smb;
        ++ind;
        if( smb != '\n'){ 
            if( smb == '-'){
                sgn = -1;
            }
            len_lst_number++;
            number = (number*10) + (smb-'0');
            read(STDIN_FILENO, &smb, 1);
        }
        else{
            number *= sgn;
            if( isPrime(number) or number <= 0){
                break;
            }
            number = 0;
            sgn = 1;
            len_lst_number = 0;
            read(STDIN_FILENO, &smb, 1);
        }
    }

    for( int i=0; i < ind-len_lst_number - 1; i++){
        write(STDOUT_FILENO,&buff[i],1);
    }
    
    return 0;
}
```

prime.cpp

```
#include <cmath>
#include "prime.h"

bool isPrime( int number) {
    if( number < 2) {
        return false;
    }

    for( int i = 2; i <= sqrt(number); i++) {
        if( number % i == 0) {
            return false;
        }
    }

    return true;
}
```

CMakeLists.txt

```
cmake_minimum_required(VERSION 3.10)
project(osi_1)

set(CMAKE_CXX_STANDARD 17)

add_executable(main scr/main.cpp)
add_executable(child scr/child_process.cpp)
```
