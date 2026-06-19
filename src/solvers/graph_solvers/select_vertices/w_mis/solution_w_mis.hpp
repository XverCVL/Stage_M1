#pragma once

#include <algorithm>

#include "../mis/solution_mis.hpp"
#include "instance_weighted_graph.hpp"
#include "solution.hpp"

namespace solverco::graph_solvers::w_mis {

using solverco::graph_solvers::InstanceWeightedGraph;
using solverco::graph_solvers::mis::SolutionMIS;
using solverco::utils::graph::vertex;
using solverco::utils::graph::weight_v;

class SolutionWMIS : public SolutionMIS {
   private:
    weight_v total_weight_ = 0;

   public:
    explicit SolutionWMIS(const Instance& instance, const std::vector<vertex>& vertices);
    explicit SolutionWMIS(const Instance& instance, const std::vector<vertex>& vertices,
                          weight_v total_weight);

    [[nodiscard]] typeOpt check_cost() override;
    [[nodiscard]] typeOpt get_value() const override;

    [[nodiscard]] std::unique_ptr<Solution> clone() const override;

    void display(std::ostream& stream) const override;
};

};  // namespace solverco::graph_solvers::w_mis
