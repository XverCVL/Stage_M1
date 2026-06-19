#include "benchmark_manager.hpp"

namespace solverco::utils::benchmark {
BenchmarkManager::BenchmarkManager(const std::vector<SolverCO*>& solvers)
    : benchmark_result_(solvers), solvers_(solvers)
{}


void solverco::utils::benchmark::BenchmarkManager::run_benchmark(
    InstanceProvider& instance_provider)
{
    while (instance_provider.has_next()) {
        auto instance = instance_provider.get_next();
        if (benchmark_result_.get_all_results().count(instance->get_name()) > 0) {
            continue;
        }
        std::cout << "-----------------------------------\n";
        std::cout << "Running for instance: " << instance->get_name() << "\n";
        std::cout << "-----------------------------------\n";
        for (auto* solver : solvers_) {
            solver->reset();
            solver->load(*instance);
            auto start = std::chrono::high_resolution_clock::now();

            solver->solve();

            auto end = std::chrono::high_resolution_clock::now();
            std::chrono::duration<double, std::milli> duration = end - start;

            std::cout << "Solver: " << solver->to_string() << ",\t Time: " << duration.count()
                      << " ms,\t "
                      << "Objective: " << solver->get_solution_manager().best_primal_value()
                      << "\n";
            benchmark_result_.add_result(instance->get_name(),
                                         new Result(solver, duration.count()));
        }
    }
}

const BenchmarkResult& solverco::utils::benchmark::BenchmarkManager::get_benchmark_result() const
{
    return benchmark_result_;
}

const std::map<std::string, std::vector<Result*>>& BenchmarkManager::get_all_results() const
{
    return benchmark_result_.get_all_results();
}

const std::vector<Result*>& BenchmarkManager::get_results(const std::string& instance_name) const
{
    return benchmark_result_.get_results(instance_name);
}

void BenchmarkManager::export_to_csv(const std::string& filename, const std::string& metric) const
{
    benchmark_result_.export_to_csv(filename, metric);
}

void BenchmarkManager::display_all_results_solutions(std::ostream& stream,
                                                     bool first_solution_only) const
{
    benchmark_result_.display_all_results_solutions(stream, first_solution_only);
}

}  // namespace solverco::utils::benchmark
