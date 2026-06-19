#include "solution_w_mis.hpp"

namespace solverco::graph_solvers::w_mis {

SolutionWMIS::SolutionWMIS(const Instance& instance, const std::vector<vertex>& vertices)
    : SolutionMIS(instance, vertices)
{
    for (vertex v : get_vertices()) {
        total_weight_ +=
            static_cast<const InstanceWeightedGraph&>(instance).get_graph().get_vertex_weight(v);
    }
}

SolutionWMIS::SolutionWMIS(const Instance& instance, const std::vector<vertex>& vertices,
                           weight_v total_weight)
    : SolutionMIS(instance, vertices), total_weight_(total_weight)
{}

typeOpt SolutionWMIS::check_cost()
{
    [[maybe_unused]] auto _ = SolutionMIS::check_cost();
    const auto& graph = static_cast<const InstanceWeightedGraph&>(get_instance()).get_graph();
    typeOpt total_weight = 0;
    for (vertex v : get_vertices()) {
        total_weight += graph.get_vertex_weight(v);
    }
    total_weight_ = total_weight;
    return get_value();
}

typeOpt SolutionWMIS::get_value() const
{
    return total_weight_;
}

std::unique_ptr<Solution> SolutionWMIS::clone() const
{
    return std::make_unique<SolutionWMIS>(get_instance(), get_vertices(), total_weight_);
}

void SolutionWMIS::display(std::ostream& stream) const
{
    stream << "Weight Max Clique of weight " << get_value() << ": {";
    for (size_t i = 0; i < get_vertices().size(); ++i) {
        stream << get_vertices()[i] + 1;
        if (i < get_vertices().size() - 1) {
            stream << ", ";
        }
    }
    stream << "}";
}

};  // namespace solverco::graph_solvers::w_mis
