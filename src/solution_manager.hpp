#pragma once

#include <algorithm>
#include <limits>
#include <string>
#include <vector>

#include "solution.hpp"

enum class OptimizationSense : std::uint8_t { MINIMIZE = 0, MAXIMIZE = 1 };

class SolutionManager {
   private:
    const Instance& instance_;
    std::string problem_name_;
    OptimizationSense opt_sense_;
    unsigned int max_nb_solutions_ = 1;
    std::vector<Solution*> solutions_;
    typeOpt dual_bound_;
    bool accept_equal_solutions_ = false;

   public:
    SolutionManager(const Instance& instance, const std::string& problem_name,
                    OptimizationSense opt_sense, unsigned int max_nb_solutions)
        : instance_(instance),
          problem_name_(problem_name),
          opt_sense_(opt_sense),
          max_nb_solutions_(max_nb_solutions),
          dual_bound_(opt_sense == OptimizationSense::MINIMIZE
                          ? std::numeric_limits<typeOpt>::max()
                          : std::numeric_limits<typeOpt>::lowest())
    {}

    ~SolutionManager()
    {
        clear_solutions();
    }

    SolutionManager(const SolutionManager&) = delete;
    SolutionManager& operator=(const SolutionManager&) = delete;
    SolutionManager(SolutionManager&&) = delete;
    SolutionManager& operator=(SolutionManager&&) = delete;

    bool contains_solution(Solution* sol)
    {
        if (solutions_.empty() || accept_equal_solutions_) {
            return false;
        }
        auto comp = [this](Solution* a, Solution* b) {
            if (opt_sense_ == OptimizationSense::MINIMIZE) {
                return a->get_value() < b->get_value();
            }
            return a->get_value() > b->get_value();
        };
        auto sol_find_range_it = std::equal_range(solutions_.begin(), solutions_.end(), sol, comp);
        for (auto it = sol_find_range_it.first; it != sol_find_range_it.second; ++it) {
            if ((*it)->equal(sol)) {
                return true;
            }
        }
        return false;
    }

    [[nodiscard]] typeOpt best_primal_value() const
    {
        if (!solutions_.empty()) {
            return solutions_[0]->get_value();
        }
        return dual_bound_;
    }

    bool insert_solution(Solution* sol, bool check = false)
    {
        if (&sol->get_instance() != &instance_ || (check && !sol->check())) {
            delete sol;
            return false;
        }

        typeOpt val = check ? sol->check_cost() : sol->get_value();
        bool is_better = solutions_.empty() || ((opt_sense_ == OptimizationSense::MINIMIZE)
                                                    ? (val < solutions_.back()->get_value())
                                                    : (val > solutions_.back()->get_value()));

        if (solutions_.size() >= max_nb_solutions_ && !is_better) {
            delete sol;
            return false;
        }

        if (contains_solution(sol)) {
            delete sol;
            return false;
        }

        auto comp = [this](Solution* a, Solution* b) {
            return (opt_sense_ == OptimizationSense::MINIMIZE) ? a->get_value() < b->get_value()
                                                               : a->get_value() > b->get_value();
        };

        solutions_.insert(std::upper_bound(solutions_.begin(), solutions_.end(), sol, comp), sol);

        if (solutions_.size() > max_nb_solutions_) {
            delete solutions_.back();
            solutions_.pop_back();
        }

        return true;
    }

    void clear_solutions()
    {
        for (auto* s : solutions_) {
            delete s;
        }
        solutions_.clear();
    }

    [[nodiscard]] const std::vector<Solution*>& get_solutions() const
    {
        return solutions_;
    }

    [[nodiscard]] const Instance& get_instance() const
    {
        return instance_;
    }

    [[nodiscard]] const std::string& get_problem_name() const
    {
        return problem_name_;
    }

    [[nodiscard]] OptimizationSense get_optimization_sense() const
    {
        return opt_sense_;
    }

    void set_dual_bound(typeOpt dual_bound)
    {
        dual_bound_ = dual_bound;
    }

    [[nodiscard]] typeOpt get_dual_bound() const
    {
        return dual_bound_;
    }

    [[nodiscard]] unsigned int get_max_nb_solutions() const
    {
        return max_nb_solutions_;
    }

    void set_accept_equal_solutions(bool accept_equal)
    {
        accept_equal_solutions_ = accept_equal;
    }

    [[nodiscard]] bool get_accept_equal_solutions() const
    {
        return accept_equal_solutions_;
    }
};
