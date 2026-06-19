#pragma once

#include "../vcp/solution_vcp.hpp"
#include "instance_weighted_graph.hpp"

namespace solverco::graph_solvers::wvcp {

using solverco::graph_solvers::vcp::SolutionVCP;
using solverco::utils::graph::color;
using solverco::utils::graph::vertex;
using solverco::utils::graph::weight_v;

class SolutionWVCP : public SolutionVCP {
   private:
    weight_v total_weight_;

   protected:
    weight_v compute_total_weight();


   public:
    explicit SolutionWVCP(const Instance& instance, const std::vector<color>& colors)
        : SolutionVCP(instance, colors), total_weight_(compute_total_weight())
    {}

    explicit SolutionWVCP(const Instance& instance, const std::vector<color>& colors,
                          typeOpt total_weight)
        : SolutionVCP(instance, colors), total_weight_(total_weight)
    {}

    explicit SolutionWVCP(const Instance& instance, const std::vector<color>& colors,
                          typeOpt total_weight, const color& max_col)
        : SolutionVCP(instance, colors, max_col), total_weight_(total_weight)
    {}

    [[nodiscard]] typeOpt check_cost() override;

    [[nodiscard]] typeOpt get_value() const override
    {
        return total_weight_;
    }

    [[nodiscard]] std::unique_ptr<Solution> clone() const override;

    void display(std::ostream& stream) const override;
};

}  // namespace solverco::graph_solvers::wvcp
