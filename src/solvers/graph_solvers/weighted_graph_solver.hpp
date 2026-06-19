#pragma once

#include "instance_weighted_graph.hpp"
#include "solver_co.hpp"

namespace solverco::graph_solvers {

class WeightedGraphSolver : public SolverCO {
   private:
    const InstanceWeightedGraph* instance_graph_;

   public:
    explicit WeightedGraphSolver(SolutionManager& solution_manager)
        : SolverCO(solution_manager),
          instance_graph_(
              &dynamic_cast<const InstanceWeightedGraph&>(solution_manager.get_instance()))
    {}
    explicit WeightedGraphSolver(const InstanceWeightedGraph& instance_graph,
                                 const std::string& problem_name,
                                 OptimizationSense optimization_sense,
                                 unsigned int max_nb_solutions)
        : SolverCO(instance_graph, problem_name, optimization_sense, max_nb_solutions),
          instance_graph_(&instance_graph)
    {}

    [[nodiscard]] const WeightedGraph& get_graph() const
    {
        return instance_graph_->get_graph();
    }

    void load(const Instance& instance) override
    {
        SolverCO::load(instance);

        instance_graph_ = &dynamic_cast<const InstanceWeightedGraph&>(instance);
    }

    [[nodiscard]] const InstanceWeightedGraph& get_instance_graph() const
    {
        return *instance_graph_;
    }
};

};  // namespace solverco::graph_solvers
