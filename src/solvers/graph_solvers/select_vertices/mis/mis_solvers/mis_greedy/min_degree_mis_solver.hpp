#pragma once

#include "mis_solver.hpp"

namespace solverco::graph_solvers::mis {

class MinDegreeMISSolver : public MISSolver {
   public:
    explicit MinDegreeMISSolver(SolutionManager& solution_manager) : MISSolver(solution_manager) {}
    explicit MinDegreeMISSolver(const InstanceGraph& instance, unsigned int max_nb_solutions = 1)
        : MISSolver(instance, max_nb_solutions)
    {}

    [[nodiscard]] bool is_stochastic() const override
    {
        return true;
    }

    void solve() override;

    [[nodiscard]] std::string to_string() const override
    {
        return "MinDegree MIS Solver";
    }
};

};  // namespace solverco::graph_solvers::mis
