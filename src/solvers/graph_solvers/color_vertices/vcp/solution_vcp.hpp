#pragma once

#include <algorithm>

#include "../color_def.hpp"
#include "instance_graph.hpp"
#include "solution.hpp"

namespace solverco::graph_solvers::vcp {

using solverco::graph_solvers::InstanceGraph;
using solverco::utils::graph::color;
using solverco::utils::graph::gint;
using solverco::utils::graph::vertex;

class SolutionVCP : public Solution {
   private:
    std::vector<color> coloring_;
    color max_color_;

   protected:
    [[nodiscard]] color compute_max_color() const;
    [[nodiscard]] std::vector<vertex> get_colored_vertices(color c) const;

   public:
    explicit SolutionVCP(const Instance& instance, const std::vector<color>& colors);
    explicit SolutionVCP(const Instance& instance, const std::vector<color>& colors,
                         const color& max_col);

    [[nodiscard]] bool check() const override;
    [[nodiscard]] typeOpt check_cost() override;
    [[nodiscard]] typeOpt get_value() const override;
    [[nodiscard]] bool equal(Solution* s) const override;

    [[nodiscard]] std::unique_ptr<Solution> clone() const override;

    void display(std::ostream& stream) const override;

    void normalize_by_min_vertex();

    [[nodiscard]] const std::vector<color>& get_colors() const
    {
        return coloring_;
    }

    [[nodiscard]] const color& get_max_color() const
    {
        return max_color_;
    }
};

};  // namespace solverco::graph_solvers::vcp
