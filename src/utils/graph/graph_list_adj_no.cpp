#include "graph_list_adj_no.hpp"

#include <algorithm>

namespace solverco::utils::graph {

GraphListAdjNO::GraphListAdjNO(const std::string& filename)
{
    read_dimacs_file(filename, *this);
}

GraphListAdjNO::GraphListAdjNO(gint nb_vertices, double proba_edge, gint seed)
{
    generate_random(nb_vertices, proba_edge, seed);
}

[[nodiscard]] gint GraphListAdjNO::nb_vertices() const
{
    return neighbors_.size();
}

[[nodiscard]] gint GraphListAdjNO::nb_edges() const
{
    return nb_edges_;
}

[[nodiscard]] bool GraphListAdjNO::is_edge(vertex v1, vertex v2) const
{
    const auto& list = neighbors_[v1];
    return std::find(list.begin(), list.end(), v2) != list.end();
}

[[nodiscard]] gint GraphListAdjNO::degree(vertex v) const
{
    return static_cast<gint>(neighbors_[v].size());
}

void GraphListAdjNO::get_neighbors(vertex v, std::vector<vertex>& neighbors) const
{
    neighbors = neighbors_[v];
}

[[nodiscard]] const std::vector<vertex>& GraphListAdjNO::neighbors(vertex v) const
{
    return neighbors_[v];
}

void GraphListAdjNO::intersect_neighbors(std::vector<vertex>& vect, vertex v1) const
{
    const auto& neigh = neighbors_[v1];
    std::vector<vertex> result(std::min(vect.size(), neigh.size()));

    auto it_end =
        std::set_intersection(vect.begin(), vect.end(), neigh.begin(), neigh.end(), result.begin());

    result.erase(it_end, result.end());
    vect = std::move(result);
}

void GraphListAdjNO::intersect_neighbors(DynamicBitset& bitset, vertex v) const
{
    const auto& neighbors = neighbors_.at(v);
    DynamicBitset result(bitset.size());

    for (vertex adj : neighbors) {
        if (adj < bitset.size() && bitset[adj]) {
            result.set(adj);
        }
    }

    bitset = std::move(result);
}

void GraphListAdjNO::intersect_neighbors(std::set<vertex>& s, vertex v1) const
{
    for (auto it = s.begin(); it != s.end();) {
        if (!is_edge(*it, v1)) {
            it = s.erase(it);
        } else {
            ++it;
        }
    }
}

void GraphListAdjNO::union_neighbors(std::vector<vertex>& vect, vertex v1) const
{
    const auto& neigh = neighbors_[v1];
    std::vector<vertex> result;
    result.reserve(vect.size() + neigh.size());
    std::set_union(vect.begin(), vect.end(), neigh.begin(), neigh.end(),
                   std::back_inserter(result));
    vect = std::move(result);
}

void GraphListAdjNO::union_neighbors(DynamicBitset& bitset, vertex v) const
{
    const auto& neighbors = neighbors_.at(v);
    for (vertex adj : neighbors) {
        bitset.set(adj);
    }
}

void GraphListAdjNO::union_neighbors(std::set<vertex>& s, vertex v1) const
{
    const auto& neigh = neighbors_[v1];
    s.insert(neigh.begin(), neigh.end());
}

void GraphListAdjNO::diff_neighbors(std::vector<vertex>& vect, vertex v1) const
{
    const auto& neigh = neighbors_[v1];
    std::vector<vertex> result;
    result.reserve(vect.size());
    std::set_difference(vect.begin(), vect.end(), neigh.begin(), neigh.end(),
                        std::back_inserter(result));
    vect = std::move(result);
}

void GraphListAdjNO::diff_neighbors(DynamicBitset& bitset, vertex v) const
{
    const auto& neighbors = neighbors_.at(v);
    for (vertex adj : neighbors) {
        bitset.reset(adj);
    }
}

void GraphListAdjNO::diff_neighbors(std::set<vertex>& s, vertex v1) const
{
    for (vertex v : neighbors_[v1]) {
        s.erase(v);
    }
}

void GraphListAdjNO::truncate(gint nb_vertices_max)
{
    if (nb_vertices_max < nb_vertices()) {
        neighbors_.resize(nb_vertices_max);
        for (auto& neigh : neighbors_) {
            neigh.erase(
                std::remove_if(neigh.begin(), neigh.end(),
                               [nb_vertices_max](vertex v) { return v >= nb_vertices_max; }),
                neigh.end());
        }
    }
}

[[nodiscard]] std::unique_ptr<Graph> GraphListAdjNO::clone(bool complementary) const
{
    if (complementary) {
        auto new_graph = std::make_unique<GraphListAdjNO>();
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
    return std::make_unique<GraphListAdjNO>(*this);
}

void GraphListAdjNO::reset(gint nb_vertices)
{
    nb_edges_ = 0;
    neighbors_.assign(nb_vertices, std::vector<vertex>());
}

void GraphListAdjNO::add_edge(vertex v1, vertex v2)
{
    neighbors_[v1].push_back(v2);
    neighbors_[v2].push_back(v1);
    nb_edges_++;
}

}  // namespace solverco::utils::graph
