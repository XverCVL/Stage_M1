#pragma once

#include "weighted_graph_solver.hpp"

namespace solverco::graph_solvers::w_max_clique {

class WMaxCliqueSolver : public WeightedGraphSolver {
   public:
    explicit WMaxCliqueSolver(SolutionManager& solution_manager)
        : WeightedGraphSolver(solution_manager)
    {}
    explicit WMaxCliqueSolver(const InstanceWeightedGraph& instance,
                              unsigned int max_nb_solutions = 1)
        : WeightedGraphSolver(instance, "WMaxCliqueSolver", OptimizationSense::MAXIMIZE,
                              max_nb_solutions)
    {}

    [[nodiscard]] std::string to_string() const override
    {
        return "WMaxCliqueSolver";
    }
};

};  // namespace solverco::graph_solvers::w_max_clique
