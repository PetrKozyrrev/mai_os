#include "prime.cpp"

#include <unistd.h>
#include <fcntl.h>

#include <iostream>
#include <string>
#include <fstream>


int main(int argc, char* argv[]){
    std::string fileData = argv[0];
    int size = atoi(argv[1]);
    int i = 0;
    
    while(i < size){
        int sign = 1;
        if(fileData[i] == '-'){
            sign = -1;
            i++;
        }
        if(fileData[i] >= '0' and fileData[i]<='9'){
            std::string t = "";
            t += fileData[i];
            i++;
            while(i<size and fileData[i] >= '0' and fileData[i] <= '9'){
                t += fileData[i];
                i++;
            }
            int r = std::stoi(t);
            r *= sign;
            if(isPrime(r)){
                 std::cout << r;
                 std::cout << " Prime" << std::endl;
            }
             else
                return 1;
        }
        else
            ++i;
    }
    return 0;
}