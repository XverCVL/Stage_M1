/*
https://github.com/fontanf/stablesolver

Parameters:
    algorithm_ = Algorithm used to solve the maximum clique
        GWMin = greedy using the vertex of minimum degree
        LocalSearch = https://github.com/fontanf/localsearchsolver
        Strong = greedy TODO

    time_limit_ = Time limit in seconds

    seed_ = seed used for the randomness in LocalSearch
*/
#pragma once

#include "max_clique_solver.hpp"

namespace solverco::graph_solvers::max_clique {

class CliqueStableSolver : public MaxCliqueSolver {
   public:
    enum class Algorithm { GWMin, LocalSearch, Strong };

   private:
    Algorithm algorithm_;
    unsigned long time_limit_;
    int seed_;

   public:
    constexpr std::string_view to_string_algorithm(Algorithm algorithm) const
    {
        switch (algorithm) {
            case Algorithm::GWMin:
                return "GWMin";
            case Algorithm::LocalSearch:
                return "LocalSearch";
            case Algorithm::Strong:
                return "Strong";
        }
        return "unknown";
    }

    explicit CliqueStableSolver(SolutionManager& solution_manager, Algorithm algorithm,
                                int seed = 0, unsigned long time_limit = 0)
        : MaxCliqueSolver(solution_manager),
          algorithm_(algorithm),
          time_limit_(time_limit),
          seed_(seed)
    {}
    explicit CliqueStableSolver(const InstanceGraph& instance, Algorithm algorithm, int seed = 0,
                                unsigned long time_limit = 0, unsigned int max_nb_solutions = 1)
        : MaxCliqueSolver(instance, max_nb_solutions),
          algorithm_(algorithm),
          time_limit_(time_limit),
          seed_(seed)
    {}

    [[nodiscard]] bool is_stochastic() const override
    {
        return true;
    }

    void solve() override;

    [[nodiscard]] std::string to_string() const override
    {
        return std::string(to_string_algorithm(algorithm_)) + " Clique StableSolver";
    }

    Algorithm get_algorithm()
    {
        return algorithm_;
    }

    unsigned long get_time_limit()
    {
        return time_limit_;
    }

    int get_seed()
    {
        return seed_;
    }
};
};  // namespace solverco::graph_solvers::max_clique
