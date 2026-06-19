#pragma once

#include <chrono>
#include <fstream>
#include <iostream>

#include "result.hpp"
#include "solver_co.hpp"

namespace solverco::utils::benchmark {

class BenchmarkResult {
   private:
    std::vector<SolverCO*> solvers_;
    std::map<std::string, std::vector<Result*>> results_;

   public:
    explicit BenchmarkResult(const std::vector<SolverCO*>& solvers);

    void add_result(const std::string& instance_name, Result* result);

    [[nodiscard]] const std::map<std::string, std::vector<Result*>>& get_all_results() const;

    [[nodiscard]] const std::vector<Result*>& get_results(const std::string& instance_name) const;

    void export_to_csv(const std::string& filename, const std::string& metric) const;  // NOLINT

    void display_all_results_solutions(std::ostream& stream,
                                       bool first_solution_only = false) const;

   private:
    [[nodiscard]] std::string get_headers(const std::string& metric) const;
};


}  // namespace solverco::utils::benchmark
