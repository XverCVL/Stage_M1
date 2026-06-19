#include "benchmark_result.hpp"

namespace solverco::utils::benchmark {
BenchmarkResult::BenchmarkResult(const std::vector<SolverCO*>& solvers) : solvers_(solvers) {}


void BenchmarkResult::add_result(const std::string& instance_name, Result* result)
{
    results_[instance_name].push_back(result);
}

const std::map<std::string, std::vector<Result*>>& BenchmarkResult::get_all_results() const
{
    return results_;
}

const std::vector<Result*>& BenchmarkResult::get_results(const std::string& instance_name) const
{
    return results_.at(instance_name);
}

void BenchmarkResult::export_to_csv(const std::string& filename,  // NOLINT
                                    const std::string& metric) const
{
    std::ofstream file(filename);
    file << get_headers(metric) << "\n";
    for (const auto& [instance_name, result_list] : results_) {
        file << instance_name << ",";
        for (const auto* result : result_list) {
            if (result->has_metric(metric)) {
                result->print(metric, file);
                file << ",";
            }
        }
        file << "\n";
    }
    std::cout << "Generated CSV file: " << filename << "\n";
}

void BenchmarkResult::display_all_results_solutions(std::ostream& stream,
                                                    bool first_solution_only) const
{
    const auto& all_results = get_all_results();

    for (const auto& [instance_name, result_list] : all_results) {
        stream << "\n";
        stream << "===================================\n";
        stream << "Instance: " << instance_name << "\n";
        stream << "===================================\n";
        for (const auto* result : result_list) {
            stream << "\n";
            stream << "Solver: " << result->get_solver_name() << "\n";
            for (const auto& solution : result->get_solutions()) {
                solution->display(stream);
                stream << "\n";
                if (first_solution_only) {
                    break;
                }
            }
        }
    }
    stream << "\n";
}

std::string BenchmarkResult::get_headers(const std::string& metric) const
{
    if (results_.empty()) {
        return "";
    }
    std::string headers = "Instance";
    auto result_list = results_.begin()->second;
    auto it = 0;
    for (const auto* solver : solvers_) {
        if (result_list[it]->has_metric(metric)) {
            headers += "," + solver->to_string();
        }
        it++;
    }
    return headers;
}

}  // namespace solverco::utils::benchmark
