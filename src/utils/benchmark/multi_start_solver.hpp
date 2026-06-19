#pragma once

#include <chrono>
#include <cmath>

#include "solver_co.hpp"

class MultiStartSolver : public SolverCO {
   private:
    SolverCO* solver_;
    unsigned int nb_iter_;
    bool need_reload_;
    double total_load_time_ = 0.0;

   public:
    MultiStartSolver(SolverCO* solver, unsigned int nb_iter, bool need_reload = false)
        : SolverCO(solver->get_solution_manager().get_instance(),
                   solver->get_solution_manager().get_problem_name(),
                   solver->get_solution_manager().get_optimization_sense(),
                   nb_iter * solver->get_solution_manager().get_max_nb_solutions()),
          solver_(solver),
          nb_iter_(nb_iter),
          need_reload_(need_reload)
    {}
    [[nodiscard]] std::string to_string() const override
    {
        return solver_->to_string() + " (" + std::to_string(nb_iter_) + ")";
    }
    [[nodiscard]] bool is_stochastic() const override
    {
        return solver_->is_stochastic();
    }
    void reset() override
    {
        SolverCO::reset();
        solver_->reset();
    }
    void load(const Instance& instance) override
    {
        SolverCO::load(instance);
        get_solution_manager().set_accept_equal_solutions(true);
        solver_->load(instance);
    }
    void solve() override
    {
        for (unsigned int i = 0; i < nb_iter_; ++i) {
            auto start = std::chrono::high_resolution_clock::now();
            solver_->reset();
            if (need_reload_) {
                solver_->load(get_solution_manager().get_instance());
            }
            auto end = std::chrono::high_resolution_clock::now();
            std::chrono::duration<double, std::milli> duration = end - start;
            total_load_time_ += duration.count();

            solver_->solve();
            for (const auto* solution : solver_->get_solution_manager().get_solutions()) {
                get_solution_manager().insert_solution(solution->clone().release());
            }
        }
    }
    void update_metrics([[maybe_unused]] MetricsMap& metrics) const override
    {
        auto time_metric = dynamic_cast<Metric<double>*>(metrics.at("time"))->get_value();
        auto new_time_metric = time_metric - total_load_time_;
        delete metrics["time"];
        metrics["time"] = new Metric<double>(new_time_metric);
        typeOpt mean_value = 0;
        typeOpt median_value =
            get_solution_manager()
                .get_solutions()[get_solution_manager().get_solutions().size() / 2]
                ->get_value();
        typeOpt variance = 0;
        for (const auto* solution : get_solution_manager().get_solutions()) {
            mean_value += solution->get_value();
            variance +=
                (solution->get_value() - median_value) * (solution->get_value() - median_value);
        }
        mean_value /= static_cast<typeOpt>(get_solution_manager().get_solutions().size());
        variance /= static_cast<typeOpt>(get_solution_manager().get_solutions().size());
        typeOpt stddev_value = std::sqrt(variance);
        metrics["mean_value"] = new Metric<typeOpt>(mean_value);
        metrics["median_value"] = new Metric<typeOpt>(median_value);
        metrics["variance_value"] = new Metric<typeOpt>(variance);
        metrics["stddev_value"] = new Metric<typeOpt>(stddev_value);
        metrics["worst_value"] =
            new Metric<typeOpt>(get_solution_manager().get_solutions().back()->get_value());
        metrics["mean_time"] = new Metric<double>(new_time_metric / static_cast<double>(nb_iter_));
    }
};
