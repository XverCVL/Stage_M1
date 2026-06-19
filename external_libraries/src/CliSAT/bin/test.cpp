#include <algorithm>
#include <random>
#include <vector>
#include <unistd.h>
#include <charconv>
#include <iostream>
#include <sys/wait.h>
#include <sstream>
#include <cstring>
#include <chrono>

std::vector<int> solve()
{
    char* graph = "brock200_1.clq";
    char* max_time = "100";
    char* degOrColor = "2";
    char* random = "0";

    char* argv[6] = {"./CliSAT_2024_07_06", graph , max_time, degOrColor, random, NULL};

    int fd[2];
    pipe(fd);

    pid_t pid = fork();
    if (pid == 0)
    {
        close(fd[0]);
        dup2(fd[1], STDOUT_FILENO);
        
        close(fd[1]);
        execv(argv[0], argv);
        exit(EXIT_FAILURE);

    }else{
        close(fd[1]);
        ssize_t bytes_read;
        ssize_t last_bytes_read;
        char buffer[4096];

        while ((bytes_read = read(fd[0], buffer, sizeof(buffer) - 1)) > 0) {
            last_bytes_read = bytes_read;
        }
        close(fd[0]);

        buffer[last_bytes_read-1] = '\0';

        std::string line;
        for (size_t i = 0; i < static_cast<size_t>(last_bytes_read); ++i) {
            if (buffer[i] == '\n') {
                line.clear();
            } else {
                line.push_back(buffer[i]);
            }
        }

        line = line.substr(7,line.rfind("  ["));

        std::vector<int> clique;
        std::istringstream iss(line);
        int value;
        while (iss >> value) {
            clique.push_back(value);
        }

        std::sort(clique.begin(), clique.end());

        waitpid(pid,NULL,0);

        return clique;
    }

}

int main(){
    std::vector<int> clique = solve();
    for(int v : clique){
        std::cout << v << " ";
    }
    std::cout << std::endl;
}
