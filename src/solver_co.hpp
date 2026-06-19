#pragma once

#include "solution_manager.hpp"

class SolverCO {
   private:
    SolutionManager* solution_manager_;
    const Instance* instance_;
    bool owns_solution_manager_ = false;

   public:
    explicit SolverCO(SolutionManager& solution_manager)
        : solution_manager_(&solution_manager), instance_(&solution_manager_->get_instance())
    {}
    explicit SolverCO(const Instance& instance, const std::string& problem_name,
                      OptimizationSense optimization_sense, unsigned int max_nb_solutions)
        : solution_manager_(
              new SolutionManager(instance, problem_name, optimization_sense, max_nb_solutions)),
          instance_(&instance),
          owns_solution_manager_(true)
    {}

    SolverCO(const SolverCO&) = delete;
    SolverCO& operator=(const SolverCO&) = delete;
    SolverCO(SolverCO&&) = delete;
    SolverCO& operator=(SolverCO&&) = delete;

    virtual ~SolverCO()
    {
        if (owns_solution_manager_) {
            delete solution_manager_;
        }
    }

    [[nodiscard]] virtual bool is_stochastic() const = 0;

    virtual void solve() = 0;

    virtual void load(const Instance& instance)
    {
        auto* solution_manager = new SolutionManager(
            instance, solution_manager_->get_problem_name(),
            solution_manager_->get_optimization_sense(), solution_manager_->get_max_nb_solutions());

        if (owns_solution_manager_) {
            delete solution_manager_;
        } else {
            owns_solution_manager_ = true;
        }

        solution_manager_ = solution_manager;
        instance_ = &instance;
    }

    virtual void reset()
    {
        clear_solutions();
    }

    [[nodiscard]] virtual std::string to_string() const = 0;

    [[nodiscard]] SolutionManager& get_solution_manager() const
    {
        return *solution_manager_;
    }

    virtual void update_metrics([[maybe_unused]] MetricsMap& metrics) const {}

    bool insert_solution(Solution* sol, bool check = false)
    {
        return solution_manager_->insert_solution(sol, check);
    }

    void clear_solutions()
    {
        solution_manager_->clear_solutions();
    }

    [[nodiscard]] const std::vector<Solution*>& get_solutions() const
    {
        return solution_manager_->get_solutions();
    }
};
