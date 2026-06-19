#pragma once

#include "graph_solver.hpp"
#include "solution_vcp.hpp"

namespace solverco::graph_solvers::vcp {

class VCPSolver : public GraphSolver {
   public:
    explicit VCPSolver(SolutionManager& solution_manager) : GraphSolver(solution_manager) {}
    explicit VCPSolver(const InstanceGraph& instance, unsigned int max_nb_solutions = 1)
        : GraphSolver(instance, "VCP", OptimizationSense::MINIMIZE, max_nb_solutions)
    {}

    [[nodiscard]] std::string to_string() const override
    {
        return "VCP Solver";
    }
};

}  // namespace solverco::graph_solvers::vcp
