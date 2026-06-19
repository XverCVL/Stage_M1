#include "clisat_solver.hpp"

#include <algorithm>
#include <filesystem>
#include <fstream>
#include <sstream>
#include <vector>

#include <fcntl.h>
#include <sys/wait.h>

static void write_graph_to_dimacs(solverco::graph_solvers::InstanceGraph const& instance,
                                  std::string const& path)
{
    using vertex = solverco::utils::graph::vertex;

    const auto& graph = instance.get_graph();
    size_t num_vertices = graph.nb_vertices();
    size_t num_edges = graph.nb_edges();

    std::ofstream f(path);
    f << "p edge " << num_vertices << " " << num_edges << "\n";
    for (size_t u = 0; u < num_vertices; u++) {
        std::vector<vertex> neighbors;
        graph.get_neighbors(u, neighbors);
        for (vertex v : neighbors)
            if (v > (vertex)u) {
                f << "e " << (u + 1) << " " << (v + 1) << "\n";  // 1-indexed
            }
    }
}

static std::string strip_ansi(std::string const& s)
{
    std::string result;
    bool in_escape = false;
    for (char c : s) {
        if (c == '\033') {
            in_escape = true;
        } else if (in_escape && c == 'm') {
            in_escape = false;
        } else if (!in_escape) {
            result += c;
        }
    }
    return result;
};

void solverco::graph_solvers::max_clique::CliSATSolver::solve()
{
    std::string clisat_path = "/tmp/CliSAT.graph";
    write_graph_to_dimacs(get_instance_graph(), clisat_path);

    std::string path_exec = get_path_exec();
    std::vector<std::string> args = {
        path_exec,
        clisat_path,
        std::to_string(get_time_limit()),
        get_ordering(),
        std::to_string(is_stochastic()),
    };
    std::vector<char*> argv;
    for (auto& arg : args)
        argv.push_back(arg.data());
    argv.push_back(nullptr);

    int fd[2];
    pipe(fd);

    pid_t pid = fork();
    if (pid == 0) {
        close(fd[0]);
        dup2(fd[1], STDOUT_FILENO);
        close(fd[1]);

        int devNull = open("/dev/null", O_WRONLY);
        dup2(devNull, STDERR_FILENO);
        close(devNull);

        chdir("/tmp");
        execv(argv[0], argv.data());
        exit(EXIT_FAILURE);
    }

    close(fd[1]);

    std::string last_line, line;
    char buffer[4096];
    ssize_t n;

    while ((n = read(fd[0], buffer, sizeof(buffer) - 1)) > 0) {
        buffer[n] = '\0';
        for (char* p = buffer; *p; ++p) {
            if (*p == '\n') {
                last_line = line;
                line.clear();
            } else {
                line += *p;
            }
        }
    }
    if (!line.empty()) {
        last_line = line;
    }
    close(fd[0]);
    waitpid(pid, NULL, 0);


    std::filesystem::remove(clisat_path);
    std::filesystem::remove("/tmp/info_clique.txt");

    last_line = strip_ansi(last_line);
    last_line = last_line.substr(0, last_line.rfind("  ["));

    std::vector<vertex> clique;
    std::istringstream iss(last_line);
    vertex value;
    while (iss >> value) {
        clique.push_back(value);
    }

    std::sort(clique.begin(), clique.end());

    auto* solution = new SolutionMaxClique(get_solution_manager().get_instance(), clique);
    if (solution->check()) {
        get_solution_manager().insert_solution(solution);
    } else {
        delete solution;
    }
}
