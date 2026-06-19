#include "graph_heavy_no.hpp"

#include <memory>

namespace solverco::utils::graph {

GraphHeavyNO::GraphHeavyNO(const std::string& filename)
{
    read_dimacs_file(filename, *this);
}

GraphHeavyNO::GraphHeavyNO(gint nb_vertices, double proba_edge, gint seed)
{
    generate_random(nb_vertices, proba_edge, seed);
}

void GraphHeavyNO::add_edge(vertex v1, vertex v2)
{
    GraphListAdjNO::add_edge(v1, v2);
    adj_matrix_[v1].set(v2);
    adj_matrix_[v2].set(v1);
}

void GraphHeavyNO::reset(gint nb_vertices)
{
    GraphListAdjNO::reset(nb_vertices);
    adj_matrix_.clear();
    adj_matrix_.resize(nb_vertices, DynamicBitset(nb_vertices));
}

[[nodiscard]] std::unique_ptr<Graph> GraphHeavyNO::clone(bool complementary) const
{
    if (complementary) {
        auto new_graph = std::make_unique<GraphHeavyNO>();
        new_graph->reset(nb_vertices());

        for (vertex v1 = 0; v1 < nb_vertices(); ++v1) {
            for (vertex v2 = v1 + 1; v2 < nb_vertices(); ++v2) {
                if (!is_edge(v1, v2)) {
                    new_graph->add_edge(v1, v2);
                }
            }
        }
        return new_graph;
    }
    return std::make_unique<GraphHeavyNO>(*this);
}

void GraphHeavyNO::intersect_neighbors(std::vector<vertex>& vect, vertex v1) const
{
    const auto& row = adj_matrix_[v1];
    auto it = std::remove_if(vect.begin(), vect.end(), [&row](vertex v) { return !row[v]; });
    vect.erase(it, vect.end());
}

void GraphHeavyNO::diff_neighbors(std::vector<vertex>& vect, vertex v1) const
{
    const auto& row = adj_matrix_[v1];
    auto it = std::remove_if(vect.begin(), vect.end(), [&row](vertex v) { return row[v]; });
    vect.erase(it, vect.end());
}

void GraphHeavyNO::intersect_neighbors(std::set<vertex>& s, vertex v1) const
{
    const auto& row = adj_matrix_[v1];
    for (auto it = s.begin(); it != s.end();) {
        if (!row[*it]) {
            it = s.erase(it);
        } else {
            ++it;
        }
    }
}

void GraphHeavyNO::diff_neighbors(std::set<vertex>& s, vertex v1) const
{
    const auto& row = adj_matrix_[v1];
    for (auto it = s.begin(); it != s.end();) {
        if (row[*it]) {
            it = s.erase(it);
        } else {
            ++it;
        }
    }
}

void GraphHeavyNO::truncate(gint nb_vertices_max)
{
    if (nb_vertices_max < nb_vertices()) {
        GraphListAdjNO::truncate(nb_vertices_max);
        adj_matrix_.resize(nb_vertices_max);
        for (auto& row : adj_matrix_) {
            row.resize(nb_vertices_max);
        }
    }
}

}  // namespace solverco::utils::graph
