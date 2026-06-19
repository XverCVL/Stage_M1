#pragma once

#include <algorithm>

#include "instance_graph.hpp"
#include "solution.hpp"

namespace solverco::graph_solvers::max_clique {

using solverco::graph_solvers::InstanceGraph;
using solverco::utils::graph::vertex;

class SolutionMaxClique : public Solution {
   private:
    std::vector<vertex> vertices_;

   public:
    explicit SolutionMaxClique(const Instance& instance, const std::vector<vertex>& vertices);
    [[nodiscard]] bool check() const override;
    [[nodiscard]] typeOpt check_cost() override;
    [[nodiscard]] typeOpt get_value() const override;
    [[nodiscard]] bool equal(Solution* s) const override;

    [[nodiscard]] std::unique_ptr<Solution> clone() const override;

    void display(std::ostream& stream) const override;

    [[nodiscard]] const std::vector<vertex>& get_vertices() const
    {
        return vertices_;
    }

    void sort()
    {
        std::sort(vertices_.begin(), vertices_.end());
    }

    const std::vector<vertex>& get_vertices()
    {
        return vertices_;
    }
};

};  // namespace solverco::graph_solvers::max_clique
