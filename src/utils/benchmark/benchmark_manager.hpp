#pragma once

#include <vector>

#include "benchmark_result.hpp"
#include "benchmark_visualizer.hpp"

namespace solverco::utils::benchmark {

class BenchmarkManager {
   private:
    BenchmarkResult benchmark_result_;
    std::vector<SolverCO*> solvers_;

   public:
    explicit BenchmarkManager(const std::vector<SolverCO*>& solvers);

    ~BenchmarkManager() = default;

    BenchmarkManager(const BenchmarkManager&) = delete;
    BenchmarkManager& operator=(const BenchmarkManager&) = delete;
    BenchmarkManager(BenchmarkManager&&) = delete;
    BenchmarkManager& operator=(BenchmarkManager&&) = delete;

    void run_benchmark(InstanceProvider& instance_provider);

    [[nodiscard]] const BenchmarkResult& get_benchmark_result() const;

    [[nodiscard]] const std::map<std::string, std::vector<Result*>>& get_all_results() const;

    [[nodiscard]] const std::vector<Result*>& get_results(const std::string& instance_name) const;

    void export_to_csv(const std::string& filename, const std::string& metric) const;  // NOLINT

    void display_all_results_solutions(std::ostream& stream,
                                       bool first_solution_only = false) const;
};

}  // namespace solverco::utils::benchmark
