#pragma once

#include "weighted_graph_solver.hpp"

namespace solverco::graph_solvers::wvcp {

class WVCPSolver : public WeightedGraphSolver {
   public:
    explicit WVCPSolver(SolutionManager& solution_manager) : WeightedGraphSolver(solution_manager)
    {}
    explicit WVCPSolver(const InstanceWeightedGraph& instance, unsigned int max_nb_solutions = 1)
        : WeightedGraphSolver(instance, "VCP", OptimizationSense::MINIMIZE, max_nb_solutions)
    {}

    [[nodiscard]] std::string to_string() const override
    {
        return "WVCP Solver";
    }
};

}  // namespace solverco::graph_solvers::wvcp
