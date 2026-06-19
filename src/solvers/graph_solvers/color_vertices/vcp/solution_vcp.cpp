#include "solution_vcp.hpp"

namespace solverco::graph_solvers::vcp {
color SolutionVCP::compute_max_color() const
{
    return *std::max_element(coloring_.begin(), coloring_.end());
}

std::vector<vertex> SolutionVCP::get_colored_vertices(color c) const
{
    std::vector<vertex> vertices;
    vertices.reserve(coloring_.size());
    if (c <= 0 || c > max_color_) {
        return vertices;
    }
    for (vertex v0 = 0; v0 < static_cast<vertex>(coloring_.size()); ++v0) {
        if (coloring_[v0] == c) {
            vertices.push_back(v0);
        }
    }
    return vertices;
}

SolutionVCP::SolutionVCP(const Instance& instance, const std::vector<color>& colors)
    : Solution(instance), coloring_(colors), max_color_(compute_max_color())
{}

SolutionVCP::SolutionVCP(const Instance& instance, const std::vector<color>& colors,
                         const color& max_col)
    : Solution(instance), coloring_(colors), max_color_(max_col)
{}


bool SolutionVCP::check() const
{
    const auto& graph = static_cast<const InstanceGraph&>(get_instance()).get_graph();
    const auto& nb_vertices = graph.nb_vertices();
    if (coloring_.size() != static_cast<size_t>(nb_vertices)) {
        return false;
    }

    for (gint v0 = 0; v0 < nb_vertices; ++v0) {
        if (coloring_[v0] == 0) {
            return false;
        }
        for (auto u : graph.neighbors(v0)) {
            if (coloring_[v0] == coloring_[u]) {
                return false;
            }
        }
    }
    return true;
}

typeOpt SolutionVCP::check_cost()
{
    max_color_ = compute_max_color();
    return get_value();
}

typeOpt SolutionVCP::get_value() const
{
    return static_cast<typeOpt>(max_color_);
}

bool SolutionVCP::equal(Solution* s) const
{
    const auto* other = dynamic_cast<const SolutionVCP*>(s);
    return coloring_ == other->coloring_;
}

std::unique_ptr<Solution> SolutionVCP::clone() const
{
    return std::make_unique<SolutionVCP>(get_instance(), coloring_, max_color_);
}

void SolutionVCP::display(std::ostream& stream) const
{
    stream << "Coloring using " << max_color_ << " colors: \n";

    for (color co = 1; co <= max_color_; ++co) {
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

void SolutionVCP::normalize_by_min_vertex()
{
    std::vector<color> remap(static_cast<size_t>(max_color_) + 1, 0);
    color current_number_color = 0;

    for (color& col : coloring_) {
        if (col <= 0) {
            continue;
        }

        if (remap[col] == 0) {
            remap[col] = ++current_number_color;
        }
        col = remap[col];
    }

    max_color_ = current_number_color;
}

};  // namespace solverco::graph_solvers::vcp
