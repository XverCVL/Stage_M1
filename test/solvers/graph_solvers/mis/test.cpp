#include <chrono>
#include <iostream>

#include "benchmark_manager.hpp"
#include "instance_graph_manager.hpp"
#include "min_degree_mis_solver.hpp"
#include "mis_bb_solver.hpp"
#include "multi_start_solver.hpp"

using solverco::algorithms::BB::BBParameters;
using solverco::algorithms::BB::SearchStrategy;
using solverco::graph_solvers::InstanceGraph;
using solverco::graph_solvers::InstanceGraphProviderFile;
using solverco::graph_solvers::MIS::MinDegreeMISSolver;
using solverco::graph_solvers::MIS::MISBBSolver;
using solverco::utils::benchmark::BenchmarkManager;
using solverco::utils::benchmark::BenchmarkVisualizer;

int main()
{
    constexpr auto MAX_SIZE_BB = 1U;
    InstanceGraph instance;
    BBParameters parameters;
    parameters.set_filtering_reduction(1).set_strategy(SearchStrategy::DFS);
    MISBBSolver solver_bb(instance, MAX_SIZE_BB, std::move(parameters));

    MinDegreeMISSolver solver_mis(instance);
    constexpr auto NB_ITER = 100;
    MultiStartSolver multi_start_solver(&solver_mis, NB_ITER, false);

    std::vector<SolverCO*> solvers{/*&solver_mis, &multi_start_solver,*/ &solver_bb};
    InstanceGraphProviderFile instance_provider({"C125.9"}, true);

    BenchmarkManager benchmark(solvers);

    benchmark.run_benchmark(instance_provider);

    benchmark.display_all_results_solutions(std::cout);

    benchmark.export_to_csv("benchmark_results_nodes_explored.csv", "nodes_processed");
    // benchmark.export_to_csv("benchmark_results_time.csv", "time");
    // benchmark.export_to_csv("benchmark_results_objective.csv", "best_value");
    // benchmark.export_to_csv("benchmark_results_mean_objective.csv", "mean_value");
    // benchmark.export_to_csv("benchmark_results_median_objective.csv", "median_value");
    // benchmark.export_to_csv("benchmark_results_stddev_objective.csv", "stddev_value");
    // benchmark.export_to_csv("benchmark_results_worst_objective.csv", "worst_value");
    // benchmark.export_to_csv("benchmark_results_mean_time.csv", "mean_time");
    // benchmark.export_to_csv("benchmark_results_variance.csv", "variance_value");
    //
    // BenchmarkVisualizer::generateBoxplot("benchmark_results_time.csv",
    // "benchmark_time_boxplot.png",
    //                                     "time");
    // BenchmarkVisualizer::generateBoxplot("benchmark_results_objective.csv",
    //                                     "benchmark_objective_boxplot.png", "best_value");
    // BenchmarkVisualizer::generatePerformanceProfile("benchmark_results_time.csv", "graphic.png",
    //                                                "time");


    return 0;
}
