#include "solution_mis.hpp"

namespace solverco::graph_solvers::mis {

SolutionMIS::SolutionMIS(const Instance& instance, const std::vector<vertex>& vertices)
    : Solution(instance), vertices_(vertices)
{}

bool SolutionMIS::check() const
{
    const auto& graph = static_cast<const InstanceGraph&>(get_instance()).get_graph();
    for (size_t i = 0; i < vertices_.size(); ++i) {
        for (size_t j = i + 1; j < vertices_.size(); ++j) {
            if (graph.is_edge(vertices_[i], vertices_[j])) {
                return false;
            }
        }
    }
    return true;
}

typeOpt SolutionMIS::check_cost()
{
    auto it = std::unique(vertices_.begin(), vertices_.end());
    vertices_.erase(it, vertices_.end());
    return get_value();
}

typeOpt SolutionMIS::get_value() const
{
    return static_cast<typeOpt>(vertices_.size());
}

bool SolutionMIS::equal(Solution* s) const
{
    const auto* other = dynamic_cast<const SolutionMIS*>(s);
    return vertices_ == other->vertices_;
}

std::unique_ptr<Solution> SolutionMIS::clone() const
{
    return std::make_unique<SolutionMIS>(get_instance(), vertices_);
}

void SolutionMIS::display(std::ostream& stream) const
{
    stream << "Independent Set of size " << vertices_.size() << ": {";
    for (size_t i = 0; i < vertices_.size(); ++i) {
        stream << vertices_[i] + 1;
        if (i < vertices_.size() - 1) {
            stream << ", ";
        }
    }
    stream << "}";
}

};  // namespace solverco::graph_solvers::mis
