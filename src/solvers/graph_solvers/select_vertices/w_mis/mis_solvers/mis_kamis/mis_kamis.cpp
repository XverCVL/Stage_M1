#include "mis_kamis.hpp"

#include <fstream>
#include <vector>

#include <fcntl.h>
#include <sys/wait.h>
#include <unistd.h>

#include "branch_and_reduce_algorithm.h"
#include "graph_access.h"
#include "graph_io.h"
#include "greedy_mis.h"
#include "local_search.h"
#include "mis/mis_config.h"
#include "online_ils.h"
#include "reduction_evolution.h"

static void write_adj_to_metis(std::vector<std::vector<int>> const& adj, size_t num_vertices,
                               std::string const& path, std::vector<int> const& weights)
{
    size_t num_edges = 0;
    for (auto const& neighbors : adj) {
        num_edges += neighbors.size();
    }
    num_edges /= 2;

    std::ofstream f(path);
    f << num_vertices << " " << num_edges << "\n";
    for (auto const& neighbors : adj) {
        for (int i = 0; i < (int)neighbors.size(); ++i) {
            if (i > 0) {
                f << " ";
            }
            f << (neighbors[i] + 1);
        }
        f << "\n";
    }
}

static void run_binary(std::string const& path_exec, std::string const& graph_path,
                       unsigned long time_limit,
                       std::vector<solverco::utils::graph::vertex>& independent_set)
{
    std::string output_path = "/tmp/mis_kamis_output.txt";

    std::vector<std::string> args = {
        path_exec,
        graph_path,
        "--output=" + output_path,
    };

    if (!(time_limit == 0)) {
        args.push_back("--time_limit=" + std::to_string(time_limit));
    }
    std::vector<char*> argv;
    for (auto& arg : args) {
        argv.push_back(arg.data());
    }

    argv.push_back(nullptr);
    pid_t pid = fork();
    if (pid == 0) {
        int devNull = open("/dev/null", O_WRONLY);
        dup2(devNull, STDOUT_FILENO);
        dup2(devNull, STDERR_FILENO);
        close(devNull);

        execv(argv[0], argv.data());

        exit(EXIT_FAILURE);

    } else {
        waitpid(pid, NULL, 0);
        std::ifstream file(output_path);

        std::string vertex;
        unsigned int index = 0;
        while (file >> vertex) {
            if (vertex.compare("1") == 0) {
                independent_set.push_back(index);
            }
            ++index;
        }
        std::remove(output_path.c_str());
    }
}

void solverco::graph_solvers::mis::MISKaMIS::solve()
{
    const auto& graph = get_instance_graph().get_graph();
    size_t num_vertices = graph.nb_vertices();
    Algorithm algorithm = get_algorithm();
    std::vector<std::vector<int>> adj(num_vertices);
    for (size_t i = 0; i < num_vertices; ++i) {
        std::vector<vertex> neighbors;
        graph.get_neighbors(i, neighbors);
        for (vertex v : neighbors) {
            adj[i].push_back(static_cast<int>(v));
        }
    }

    graph_access G;
    if (algorithm != Algorithm::KERNEL) {
        std::string tmp_path = "/tmp/mis_kamis_tmp.graph";
        write_adj_to_metis(adj, num_vertices, tmp_path);
        graph_io::readGraphWeighted(G, tmp_path);
        std::remove(tmp_path.c_str());
    }

    MISConfig mis_config;

    mis_config.time_limit = get_time_limit();
    mis_config.seed = 0;
    mis_config.imbalance = 0.03;
    mis_config.population_size = 50;
    mis_config.number_of_k_partitions = 10;
    mis_config.number_of_k_separators = 10;
    mis_config.number_of_separators = 10;
    mis_config.number_of_partitions = 10;
    mis_config.multiway_blocks = 64;
    mis_config.ils_iterations = 15000;
    mis_config.repetitions = 50;
    mis_config.pool_threshold = 250;
    mis_config.reduction_threshold = 350;
    mis_config.flip_coin = 3;
    mis_config.use_multiway_vc = false;
    mis_config.extract_best_nodes = true;
    mis_config.enable_tournament_selection = true;
    mis_config.tournament_size = 2;
    mis_config.insert_threshold = 150;
    mis_config.remove_fraction = 0.10;
    mis_config.diversify = false;
    mis_config.print_repetition = false;

    std::vector<vertex> independent_set;

    if (algorithm == Algorithm::KERNEL) {
        branch_and_reduce_algorithm solver(adj, num_vertices);

        timer t;
        std::vector<bool> is_member_map(num_vertices, false);

        if (mis_config.time_limit <= 0) {
            mis_config.time_limit = 1000;
        }
        solver.solve(t, mis_config.time_limit);

        solver.get_solved_is(is_member_map);

        for (size_t i = 0; i < num_vertices; ++i) {
            if (is_member_map[i]) {
                independent_set.push_back(static_cast<vertex>(i));
            }
        }
    } else if (algorithm == Algorithm::OnlineMIS) {
        // Manual check for a starting node if you need one
        NodeID start_node = UINT_MAX;
        for (NodeID v = 0; v < G.number_of_nodes(); ++v) {
            if (G.getPartitionIndex(v) == 0) {  // Find first unassigned node
                start_node = v;
                break;
            }
        }

        online_ils solver;
        if (start_node != UINT_MAX) {
            solver.perform_ils(mis_config, G, 10);
        }

        for (NodeID v = 0; v < G.number_of_nodes(); ++v) {
            if (G.getPartitionIndex(v) == 1) {
                independent_set.push_back(static_cast<vertex>(v));
            }
        }
    } else if (algorithm == Algorithm::GREEDY) {
        greedy_mis greedy;
        greedy.initial_partition(mis_config.seed, G);

        for (NodeID v = 0; v < G.number_of_nodes(); ++v) {
            if (G.getPartitionIndex(v) == 1) {
                independent_set.push_back(static_cast<vertex>(v));
            }
        }
    } else if (algorithm == Algorithm::ReduMIS) {
        reduction_evolution<branch_and_reduce_algorithm> solver;
        std::vector<bool> is_vector(G.number_of_nodes(), false);
        std::vector<NodeID> best_nodes;
        solver.perform_mis_search(mis_config, G, is_vector, best_nodes);
        for (NodeID v = 0; v < G.number_of_nodes(); ++v) {
            if (is_vector[v]) {
                independent_set.push_back(static_cast<vertex>(v));
            }
        }
    } else {
        std::string path_exec;
        if (algorithm == Algorithm::Struction) {
            path_exec = get_path_exec().append("struction");
        } else if (algorithm == Algorithm::MMWIS) {
            path_exec = get_path_exec().append("mmwis");
        } else if (algorithm == Algorithm::WeightedBranchReduce) {
            path_exec = get_path_exec().append("weighted_branch_reduce");
        } else if (algorithm == Algorithm::WeightedLocalSearch) {
            path_exec = get_path_exec().append("weighted_local_search");
        }

        std::string graph_path = "/tmp/tmp.graph";
        write_adj_to_metis(adj, num_vertices, graph_path);

        unsigned long time_limit = get_time_limit();

        run_binary(path_exec, graph_path, time_limit, independent_set);
        //     std::vector<std::string> args = {
        //         path_exec,
        //         graph_path,
        //         "--output=output.txt",
        //     };

        //     unsigned long time_limit = get_time_limit();
        //     if (!(time_limit == 0)) {
        //         args.push_back("--time_limit=" + std::to_string(time_limit));
        //     }
        //     std::vector<char*> argv;
        //     for (auto& arg : args) {
        //         argv.push_back(arg.data());
        //     }

        //     argv.push_back(nullptr);
        //     pid_t pid = fork();
        //     if (pid == 0) {
        //         int devNull = open("/dev/null", O_WRONLY);
        //         dup2(devNull, STDOUT_FILENO);
        //         dup2(devNull, STDERR_FILENO);
        //         close(devNull);

        //         execv(argv[0], argv.data());
        //         exit(EXIT_FAILURE);

        //     } else {
        //         waitpid(pid, NULL, 0);
        //         std::ifstream file("output.txt");

        //         std::string vertex;
        //         unsigned int index = 0;
        //         while (file >> vertex) {
        //             if (vertex.compare("1") == 0) {
        //                 independent_set.push_back(index);
        //             }
        //             ++index;
        //         }
        //     }

        //     std::remove("output.txt");
        std::remove(graph_path.c_str());
    }

    auto* solution = new SolutionMIS(get_solution_manager().get_instance(), independent_set);
    if (solution->check()) {
        get_solution_manager().insert_solution(solution);
    } else {
        delete solution;
    }
}
