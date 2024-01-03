#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <signal.h>

#include <iostream>
#include <string>


int main(int argc, char *argv[]) {
    
    if(argc != 2){
        std::cerr << "No name test file" << std::endl;
        return 1;
    }

     // Путь дочернего процесса
    const char* CHILD_PATH = "child";  

    // Путь тестируемого файла
    std::string path {"../../test/"};
    std::string name {argv[1]};
    path += name;


    // Открываем файл для чтения и записи
    int fd = open(path.c_str(), O_RDONLY);
    if (fd == -1) {
        std::cerr << "Error not open file" << std::endl;
        return 1;
    }

    // Определяем размер файла
    struct stat fileStats;
    if (fstat(fd, &fileStats) == -1) {
        std::cerr << "Failed to get file information" << std::endl;
        close(fd);
        return 1;
    }

    // Выполняем отображение файла в память
    char* fileData = (char*)mmap(NULL, fileStats.st_size, PROT_READ, MAP_SHARED, fd, 0);
    if (fileData == MAP_FAILED) {
        std::cerr << "Failed to map the file into memory" << std::endl;
        close(fd);
        return 1;
    }

    // Create fork
    pid_t pid = fork();

    if( pid == -1){
        perror("Pid errror\n");
        return 1;
    }
    if( pid == 0){ 
        // child process 
        std::string size = std::to_string(fileStats.st_size);
        execl(CHILD_PATH, fileData,size.c_str(),NULL);
        exit(EXIT_SUCCESS);
    }
    else{
        int status;
        waitpid(pid, &status, 0);
        if (WIFEXITED(status)) {
            int exitStatus = WEXITSTATUS(status);
            std::cout << "\nChild process exited with status: " << exitStatus << std::endl;
        } else if (WIFSIGNALED(status)) {
            int signalNumber = WTERMSIG(status);
            std::cout << "\nChild process was terminated by signal: " << signalNumber << std::endl;
        }
    }

    // Закрываем файл и освобождаем память
    if (munmap(fileData, fileStats.st_size) == -1) {
        std::cerr << "Failed to free memory" << std::endl;
    }

    close(fd);
    
    return 0;
}
