/*
https://github.com/fontanf/stablesolver

Parameters:
    algorithm_ = Algorithm used to solve the MIS
        GWMax = greedy using the vertex of maximum degree
        GWMin = greedy using the vertex of minimum degree
        GWMin2 = greedy using the vertex of minimum degree TODO
        LargeNeighborhoodSearch = TODO
        LocalSearch = https://github.com/fontanf/localsearchsolver
        LocalSearchRW1 = Unweighted Only
        LocalSearchRW2 = Unweighted Only
        Strong = greedy TODO

    time_limit_ = Time limit in seconds

    seed_ = seed used for the randomness in LocalSearch
*/

#pragma once

#include "mis_solver.hpp"

namespace solverco::graph_solvers::mis {

class MISStableSolver : public MISSolver {
   public:
    enum class Algorithm {
        GWMax,
        GWMin,
        GWMin2,
        LargeNeighborhoodSearch,
        LocalSearch,
        LocalSearchRW1,
        LocalSearchRW2,
        Strong
    };

   private:
    Algorithm algorithm_;
    const unsigned long time_limit_;
    int seed_;

   public:
    constexpr std::string_view to_string_algorithm(Algorithm theme) const
    {
        switch (theme) {
            case Algorithm::GWMax:
                return "GWMax";
            case Algorithm::GWMin:
                return "GWMin";
            case Algorithm::GWMin2:
                return "GWMin2";
            case Algorithm::LargeNeighborhoodSearch:
                return "LargeNeighborhoodSearch";
            case Algorithm::LocalSearch:
                return "Local Search";
            case Algorithm::LocalSearchRW1:
                return "LocalSearchRW1";
            case Algorithm::LocalSearchRW2:
                return "LocalSearchRW2";
            case Algorithm::Strong:
                return "Strong";
        }
        return "unknown";
    }

   public:
    explicit MISStableSolver(SolutionManager& solution_manager, Algorithm algorithm, int seed = 0,
                             unsigned long time_limit_ = 0)
        : MISSolver(solution_manager), algorithm_(algorithm), time_limit_(time_limit_), seed_(seed)
    {}
    explicit MISStableSolver(const InstanceGraph& instance, Algorithm algorithm, int seed = 0,
                             unsigned long time_limit_ = 0, unsigned int max_nb_solutions = 1)
        : MISSolver(instance, max_nb_solutions),
          algorithm_(algorithm),
          time_limit_(time_limit_),
          seed_(seed)
    {}

    [[nodiscard]] bool is_stochastic() const override
    {
        return true;
    }

    void solve() override;

    [[nodiscard]] std::string to_string() const override
    {
        return std::string(to_string_algorithm(algorithm_)) + " MIS StableSolver";
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

};  // namespace solverco::graph_solvers::mis
