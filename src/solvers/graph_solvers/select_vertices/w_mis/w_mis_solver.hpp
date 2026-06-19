#pragma once

#include "weighted_graph_solver.hpp"

namespace solverco::graph_solvers::w_mis {

class WMISSolver : public WeightedGraphSolver {
   public:
    explicit WMISSolver(SolutionManager& solution_manager) : WeightedGraphSolver(solution_manager)
    {}
    explicit WMISSolver(const InstanceWeightedGraph& instance, unsigned int max_nb_solutions = 1)
        : WeightedGraphSolver(instance, "WMISSolver", OptimizationSense::MAXIMIZE, max_nb_solutions)
    {}

    [[nodiscard]] std::string to_string() const override
    {
        return "WMISSolver";
    }
};

};  // namespace solverco::graph_solvers::w_mis
