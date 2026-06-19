#pragma once

#include "graph_solver.hpp"
#include "solution_mis.hpp"

namespace solverco::graph_solvers::mis {

class MISSolver : public GraphSolver {
   public:
    explicit MISSolver(SolutionManager& solution_manager) : GraphSolver(solution_manager) {}
    explicit MISSolver(const InstanceGraph& instance, unsigned int max_nb_solutions = 1)
        : GraphSolver(instance, "MIS", OptimizationSense::MAXIMIZE, max_nb_solutions)
    {}

    [[nodiscard]] std::string to_string() const override
    {
        return "MIS Solver";
    }
};

}  // namespace solverco::graph_solvers::mis
