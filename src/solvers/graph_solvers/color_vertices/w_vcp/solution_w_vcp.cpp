#include "solution_w_vcp.hpp"

#include <algorithm>

namespace solverco::graph_solvers::wvcp {

weight_v SolutionWVCP::compute_total_weight()
{
    const auto& colors = get_colors();
    if (colors.empty()) {
        return 0;
    }

    const auto& weighted_instance = dynamic_cast<const InstanceWeightedGraph&>(get_instance());
    const auto& graph = weighted_instance.get_graph();

    std::vector<weight_v> max_weights(get_max_color() + 1, 0);

    for (size_t v0 = 0; v0 < colors.size(); ++v0) {
        color c = colors[v0];
        if (c > 0) {
            weight_v w = graph.get_vertex_weight(v0);
            max_weights[c] = std::max(w, max_weights[c]);
        }
    }

    weight_v sum = 0;
    for (size_t i = 1; i < max_weights.size(); ++i) {
        sum += max_weights[i];
    }

    return sum;
}

typeOpt SolutionWVCP::check_cost()
{
    total_weight_ = compute_total_weight();
    return total_weight_;
}

std::unique_ptr<Solution> SolutionWVCP::clone() const
{
    return std::make_unique<SolutionWVCP>(get_instance(), get_colors(), total_weight_,
                                          get_max_color());
}

void SolutionWVCP::display(std::ostream& stream) const
{
    stream << "WVCP Cost: " << total_weight_ << " | Colors: " << get_max_color() << "\n";

    for (color co = 1; co <= get_max_color(); ++co) {
        stream << "color " << co << " = {";

        bool first = true;
        for (const auto& vertex : get_colored_vertices(co)) {
            if (!first) {
                stream << ", ";
                first = false;
            }
            stream << vertex + 1;
        }

        stream << "}\n";
    }
}

}  // namespace solverco::graph_solvers::wvcp
