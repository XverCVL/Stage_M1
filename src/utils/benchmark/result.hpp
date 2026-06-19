#pragma once

#include <memory>
#include <string>
#include <vector>

#include "metric.hpp"
#include "solution.hpp"
#include "solver_co.hpp"

namespace solverco::utils::benchmark {

class Result {
   private:
    std::string solver_name_;
    std::vector<std::unique_ptr<Solution>> solutions_;
    MetricsMap metrics_;

   public:
    Result(const SolverCO* solver, double duration) : solver_name_(solver->to_string())
    {
        for (const auto* solution : solver->get_solution_manager().get_solutions()) {
            solutions_.emplace_back(solution->clone());
        }
        metrics_["time"] = new Metric<double>(duration);
        metrics_["best_value"] =
            new Metric<typeOpt>(solver->get_solution_manager().best_primal_value());
        solver->update_metrics(metrics_);
    }
    ~Result()
    {
        for (auto& [_, metric] : metrics_) {
            delete metric;
        }
    }

    Result(const Result&) = delete;
    Result& operator=(const Result&) = delete;
    Result(Result&&) = delete;
    Result& operator=(Result&&) = delete;

    [[nodiscard]] const std::string& get_solver_name() const
    {
        return solver_name_;
    }

    [[nodiscard]] const std::vector<std::unique_ptr<Solution>>& get_solutions() const
    {
        return solutions_;
    }

    [[nodiscard]] bool has_metric(const std::string& metric) const
    {
        return metrics_.find(metric) != metrics_.end();
    }

    bool print(const std::string& metric, std::ostream& stream) const
    {
        if (!has_metric(metric)) {
            return false;
        }
        metrics_.at(metric)->print(stream);
        return true;
    }

    [[nodiscard]] IMetric* get_metric(const std::string& metric) const
    {
        if (!has_metric(metric)) {
            return nullptr;
        }
        return metrics_.at(metric);
    }
};

using BenchmarkResults = std::vector<Result*>;

}  // namespace solverco::utils::benchmark
