#include "clique_stablesolver.hpp"

#include <algorithm>
#include <random>
#include <sstream>
#include <vector>

#include <sys/wait.h>
#include <unistd.h>

#include "optimizationtools/graph/adjacency_list_graph.hpp"
#include "stablesolver/clique/algorithms/greedy.hpp"
#include "stablesolver/clique/algorithms/local_search.hpp"
#include "stablesolver/clique/algorithms/milp.hpp"
#include "stablesolver/clique/instance.hpp"

void solverco::graph_solvers::max_clique::CliqueStableSolver::solve()
{
    const auto& graph = get_instance_graph().get_graph();

    auto builder = optimizationtools::AdjacencyListGraphBuilder();
    for (size_t i = 0; i < graph.nb_vertices(); ++i) {
        builder.add_vertex(i);

        std::vector<vertex> neighbors;
        graph.get_neighbors(i, neighbors);

        for (vertex v : neighbors) {
            if (i > v) {
                builder.add_edge(i, v);
            }
        }
    }

    builder.remove_duplicate_edges();
    auto stableSolverGraph =
        std::make_shared<optimizationtools::AdjacencyListGraph>(builder.build());
    stablesolver::clique::Instance instance(stableSolverGraph);
    stablesolver::clique::Output output(instance);

    unsigned long time_limit = get_time_limit();
    stablesolver::clique::Parameters parameters;
    parameters.verbosity_level = 0;
    parameters.timer.set_time_limit(time_limit);

    Algorithm algorithm = get_algorithm();
    if (algorithm == Algorithm::GWMin) {
        output = greedy_gwmin(instance, parameters);

    } else if (algorithm == Algorithm::LocalSearch) {
        stablesolver::clique::LocalSearchParameters local_search_parameters;
        local_search_parameters.verbosity_level = 0;
        local_search_parameters.timer.set_time_limit(time_limit);
        local_search_parameters.number_of_threads = 1;

        int seed = get_seed();
        std::mt19937_64 generator(seed);
        output = local_search(instance, generator, local_search_parameters);

    } else if (algorithm == Algorithm::Strong) {
        output = greedy_strong(instance, parameters);
    }

    const stablesolver::clique::Solution& best_sol = output.solution;

    std::vector<vertex> independent_set;
    for (stablesolver::clique::VertexId v = 0;
         v < best_sol.instance().graph()->number_of_vertices(); ++v) {
        if (best_sol.contains(v)) {
            independent_set.push_back(static_cast<vertex>(v));
        }
    }

    auto* solution = new SolutionMaxClique(get_solution_manager().get_instance(), independent_set);
    if (solution->check()) {
        get_solution_manager().insert_solution(solution);
    } else {
        delete solution;
    }
}
