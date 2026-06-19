/*
Philippe Galinier, le 26 octobre 2004
algorithme TabuCOL

Parameters :
    time_limit_ = Time limit in seconds
    seed_ = Seed used for the randomness
*/

#pragma once

#include <random>
#include <vector>

#include "dynamic_bitset.hpp"
#include "solution_vcp.hpp"
#include "vcp_solver.hpp"

namespace solverco::graph_solvers::vcp {
using Bitset = solverco::utils::containers::DynamicBitset;

class VCPTabuCol : public VCPSolver {
   private:
    const unsigned long time_limit_;
    const int seed_;
    const unsigned int max_iter_;
    const double alpha_;

   public:
    explicit VCPTabuCol(SolutionManager& solution_manager, int seed, unsigned int max_iter,
                        double alpha, unsigned long time_limit_ = 0)
        : VCPSolver(solution_manager),
          time_limit_(time_limit_),
          seed_(seed),
          max_iter_(max_iter),
          alpha_(alpha)
    {}
    explicit VCPTabuCol(const InstanceGraph& instance, int seed, unsigned int max_iter,
                        double alpha, unsigned long time_limit_ = 0,
                        unsigned int max_nb_solutions = 1)
        : VCPSolver(instance, max_nb_solutions),
          time_limit_(time_limit_),
          seed_(seed),
          max_iter_(max_iter),
          alpha_(alpha)
    {}

    [[nodiscard]] bool is_stochastic() const override
    {
        return true;
    }

    void solve() override;

    [[nodiscard]] std::string to_string() const override
    {
        return "TabuCol VCP Solver";
    }

    unsigned long get_time_limit()
    {
        return time_limit_;
    }

    int get_seed()
    {
        return seed_;
    }

    unsigned int get_max_iter()
    {
        return max_iter_;
    }

    double get_alpha()
    {
        return alpha_;
    }
};

};  // namespace solverco::graph_solvers::vcp
