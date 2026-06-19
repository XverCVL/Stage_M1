#pragma once

#include <algorithm>

#include "../max_clique/solution_max_clique.hpp"
#include "instance_weighted_graph.hpp"
#include "solution.hpp"

namespace solverco::graph_solvers::w_max_clique {

using solverco::graph_solvers::InstanceWeightedGraph;
using solverco::graph_solvers::max_clique::SolutionMaxClique;
using solverco::utils::graph::vertex;
using solverco::utils::graph::weight_v;

class SolutionWMaxClique : public SolutionMaxClique {
   private:
    weight_v total_weight_ = 0;

   public:
    explicit SolutionWMaxClique(const Instance& instance, const std::vector<vertex>& vertices);
    explicit SolutionWMaxClique(const Instance& instance, const std::vector<vertex>& vertices,
                                weight_v total_weight);

    [[nodiscard]] typeOpt check_cost() override;
    [[nodiscard]] typeOpt get_value() const override;

    [[nodiscard]] std::unique_ptr<Solution> clone() const override;

    void display(std::ostream& stream) const override;
};

};  // namespace solverco::graph_solvers::w_max_clique
