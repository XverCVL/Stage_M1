#include "mis_stablesolver.hpp"

#include <vector>

#include <fcntl.h>
#include <stablesolver/stable/algorithm.hpp>
#include <stablesolver/stable/algorithms/greedy.hpp>
#include <stablesolver/stable/algorithms/large_neighborhood_search.hpp>
#include <stablesolver/stable/algorithms/local_search.hpp>
#include <stablesolver/stable/algorithms/local_search_row_weighting.hpp>
#include <stablesolver/stable/instance.hpp>
#include <stablesolver/stable/instance_builder.hpp>
#include <stablesolver/stable/solution.hpp>
#include <sys/wait.h>
#include <unistd.h>

void solverco::graph_solvers::mis::MISStableSolver::solve()
{
    const auto& graph = get_instance_graph().get_graph();
    stablesolver::stable::InstanceBuilder builder;

    for (size_t i = 0; i < graph.nb_vertices(); ++i) {
        builder.add_vertex(1);
        std::vector<vertex> neighbors;
        graph.get_neighbors(i, neighbors);

        for (vertex v : neighbors) {
            if (i > v) {
                builder.add_edge(i, v);
            }
        }
    }
    Algorithm algorithm = get_algorithm();
    if (algorithm == Algorithm::LocalSearchRW1 || algorithm == Algorithm::LocalSearchRW2) {
        builder.set_unweighted();
    }
    auto instance = builder.build();
    stablesolver::stable::Output output(instance);

    unsigned long time_limit = get_time_limit();
    stablesolver::stable::GreedyParameters parameters;
    parameters.verbosity_level = 0;
    parameters.timer.set_time_limit(time_limit);
    parameters.reduction_parameters.reduce = false;

    if (algorithm == Algorithm::GWMax) {
        output = stablesolver::stable::greedy_gwmax(instance, parameters);
    } else if (algorithm == Algorithm::GWMin) {
        output = stablesolver::stable::greedy_gwmin(instance, parameters);
    } else if (algorithm == Algorithm::GWMin2) {
        output = stablesolver::stable::greedy_gwmin2(instance, parameters);
    } else if (algorithm == Algorithm::Strong) {
        output = stablesolver::stable::greedy_strong(instance, parameters);
    } else if (algorithm == Algorithm::LocalSearch) {
        stablesolver::stable::LocalSearchParameters local_search_parameters;
        local_search_parameters.verbosity_level = 0;
        local_search_parameters.timer = optimizationtools::Timer();
        local_search_parameters.timer.set_time_limit(time_limit);
        output = stablesolver::stable::local_search(instance, local_search_parameters);
    } else if (algorithm == Algorithm::LocalSearchRW1) {
        stablesolver::stable::LocalSearchRowWeighting1Parameters local_row_1_parameters;
        local_row_1_parameters.verbosity_level = 0;
        local_row_1_parameters.timer.set_time_limit(time_limit);

        std::mt19937_64 generator(get_seed());
        output = stablesolver::stable::local_search_row_weighting_1(instance, generator,
                                                                    local_row_1_parameters);

    } else if (algorithm == Algorithm::LocalSearchRW2) {
        stablesolver::stable::LocalSearchRowWeighting2Parameters local_row_2_parameters;
        local_row_2_parameters.verbosity_level = 0;
        local_row_2_parameters.timer.set_time_limit(time_limit);

        std::mt19937_64 generator(get_seed());
        output = stablesolver::stable::local_search_row_weighting_2(instance, generator,
                                                                    local_row_2_parameters);
    } else if (algorithm == Algorithm::LargeNeighborhoodSearch) {
        stablesolver::stable::LargeNeighborhoodSearchParameters large_neighborhood_parameters;
        large_neighborhood_parameters.verbosity_level = 0;
        large_neighborhood_parameters.timer.set_time_limit(time_limit);

        output = stablesolver::stable::large_neighborhood_search(instance,
                                                                 large_neighborhood_parameters);
    }

    const stablesolver::stable::Solution& best_sol = output.solution;

    std::vector<vertex> independent_set;
    for (stablesolver::stable::VertexId v = 0; v < instance.number_of_vertices(); ++v) {
        if (best_sol.contains(v)) {
            independent_set.push_back(static_cast<vertex>(v));
        }
    }

    auto* solution = new SolutionMIS(get_solution_manager().get_instance(), independent_set);
    if (solution->check()) {
        get_solution_manager().insert_solution(solution);
    } else {
        delete solution;
    }
}
