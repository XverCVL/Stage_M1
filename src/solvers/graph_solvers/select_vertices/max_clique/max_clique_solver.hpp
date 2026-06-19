#pragma once

#include "graph_solver.hpp"
#include "solution_max_clique.hpp"

namespace solverco::graph_solvers::max_clique {

class MaxCliqueSolver : public GraphSolver {
   public:
    explicit MaxCliqueSolver(SolutionManager& solution_manager) : GraphSolver(solution_manager) {}
    explicit MaxCliqueSolver(const InstanceGraph& instance, unsigned int max_nb_solutions = 1)
        : GraphSolver(instance, "MaxClique", OptimizationSense::MAXIMIZE, max_nb_solutions)
    {}

    [[nodiscard]] std::string to_string() const override
    {
        return "MaxClique Solver";
    }
};

};  // namespace solverco::graph_solvers::max_clique
