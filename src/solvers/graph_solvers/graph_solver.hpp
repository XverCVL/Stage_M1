#pragma once

#include "instance_graph.hpp"
#include "solver_co.hpp"

namespace solverco::graph_solvers {

class GraphSolver : public SolverCO {
   private:
    const InstanceGraph* instance_graph_;

   public:
    explicit GraphSolver(SolutionManager& solution_manager)
        : SolverCO(solution_manager),
          instance_graph_(&dynamic_cast<const InstanceGraph&>(solution_manager.get_instance()))
    {}
    explicit GraphSolver(const InstanceGraph& instance_graph, const std::string& problem_name,
                         OptimizationSense optimization_sense, unsigned int max_nb_solutions)
        : SolverCO(instance_graph, problem_name, optimization_sense, max_nb_solutions),
          instance_graph_(&instance_graph)
    {}

    [[nodiscard]] const Graph& get_graph() const
    {
        return instance_graph_->get_graph();
    }

    void load(const Instance& instance) override
    {
        SolverCO::load(instance);

        instance_graph_ = &dynamic_cast<const InstanceGraph&>(instance);
    }

    [[nodiscard]] const InstanceGraph& get_instance_graph() const
    {
        return *instance_graph_;
    }
};

}  // namespace solverco::graph_solvers
