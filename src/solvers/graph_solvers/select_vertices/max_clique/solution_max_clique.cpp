#include "solution_max_clique.hpp"

namespace solverco::graph_solvers::max_clique {

SolutionMaxClique::SolutionMaxClique(const Instance& instance, const std::vector<vertex>& vertices)
    : Solution(instance), vertices_(vertices)
{}

bool SolutionMaxClique::check() const
{
    const auto& graph = static_cast<const InstanceGraph&>(get_instance()).get_graph();
    for (size_t i = 0; i < vertices_.size(); ++i) {
        for (size_t j = i + 1; j < vertices_.size(); ++j) {
            if (!graph.is_edge(vertices_[i], vertices_[j])) {
                return false;
            }
        }
    }
    return true;
}

typeOpt SolutionMaxClique::check_cost()
{
    auto it = std::unique(vertices_.begin(), vertices_.end());
    vertices_.erase(it, vertices_.end());
    return get_value();
}

typeOpt SolutionMaxClique::get_value() const
{
    return static_cast<typeOpt>(vertices_.size());
}

bool SolutionMaxClique::equal(Solution* s) const
{
    const auto* other = dynamic_cast<const SolutionMaxClique*>(s);
    return vertices_ == other->vertices_;
}

std::unique_ptr<Solution> SolutionMaxClique::clone() const
{
    return std::make_unique<SolutionMaxClique>(get_instance(), vertices_);
}

void SolutionMaxClique::display(std::ostream& stream) const
{
    stream << "Max Clique of size " << vertices_.size() << ": {";
    for (size_t i = 0; i < vertices_.size(); ++i) {
        stream << vertices_[i] + 1;
        if (i < vertices_.size() - 1) {
            stream << ", ";
        }
    }
    stream << "}";
}

};  // namespace solverco::graph_solvers::max_clique
