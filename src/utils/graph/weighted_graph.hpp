#pragma once

#include <algorithm>
#include <memory>

#include "graph.hpp"

namespace solverco::utils::graph {

constexpr int MAX_VERTEX_MODULO = 200;

class WeightedVertices {
   public:
    WeightedVertices() = default;
    virtual ~WeightedVertices() = default;
    WeightedVertices(const WeightedVertices&) = default;
    WeightedVertices& operator=(const WeightedVertices&) = default;
    WeightedVertices(WeightedVertices&&) = default;
    WeightedVertices& operator=(WeightedVertices&&) = default;

    virtual weight_v get([[maybe_unused]] const std::unique_ptr<Graph>& g, vertex v)
    {
        return 1 + (v % MAX_VERTEX_MODULO);
    }
};

class WeightedEdges {
   public:
    WeightedEdges() = default;
    virtual ~WeightedEdges() = default;
    WeightedEdges(const WeightedEdges&) = default;
    WeightedEdges& operator=(const WeightedEdges&) = default;
    WeightedEdges(WeightedEdges&&) = default;
    WeightedEdges& operator=(WeightedEdges&&) = default;

    virtual weight_e get([[maybe_unused]] const std::unique_ptr<Graph>& g, vertex v1, vertex v2)
    {
        return std::min<weight_e>(
            static_cast<weight_e>((v2 - v1) * (v2 - v1)),
            static_cast<weight_e>((g->nb_vertices() + std::min(v1, v2) - std::max(v1, v2)) *
                                  (g->nb_vertices() + std::min(v1, v2) - std::max(v1, v2))));
    }
};

class WeightedGraph : public Graph {
   private:
    std::unique_ptr<Graph> g_;
    std::unique_ptr<WeightedVertices> vertex_weights_;
    std::unique_ptr<WeightedEdges> edge_weights_;

   public:
    explicit WeightedGraph(
        std::unique_ptr<Graph> g,
        std::unique_ptr<WeightedVertices> vertex_weights = std::make_unique<WeightedVertices>(),
        std::unique_ptr<WeightedEdges> edge_weights = std::make_unique<WeightedEdges>())
        : g_(std::move(g)),
          vertex_weights_(std::move(vertex_weights)),
          edge_weights_(std::move(edge_weights))
    {}

    [[nodiscard]] gint nb_vertices() const override
    {
        return g_->nb_vertices();
    }

    [[nodiscard]] gint nb_edges() const override
    {
        return g_->nb_edges();
    }

    [[nodiscard]] bool is_edge(vertex v1, vertex v2) const override
    {
        return g_->is_edge(v1, v2);
    }

    [[nodiscard]] gint degree(vertex v) const override
    {
        return g_->degree(v);
    }

    [[nodiscard]] const std::vector<vertex>& neighbors(vertex v) const override
    {
        return g_->neighbors(v);
    }

    void get_neighbors(vertex v, std::vector<vertex>& neighbors) const override
    {
        g_->get_neighbors(v, neighbors);
    }

    void intersect_neighbors(std::vector<vertex>& intersect_vector, vertex v) const override
    {
        g_->intersect_neighbors(intersect_vector, v);
    }

    void intersect_neighbors(DynamicBitset& intersect_bitset, vertex v) const override
    {
        g_->intersect_neighbors(intersect_bitset, v);
    }

    void intersect_neighbors(std::set<vertex>& intersect_set, vertex v) const override
    {
        g_->intersect_neighbors(intersect_set, v);
    }

    void union_neighbors(std::vector<vertex>& union_vector, vertex v) const override
    {
        g_->union_neighbors(union_vector, v);
    }

    void union_neighbors(DynamicBitset& union_bitset, vertex v) const override
    {
        g_->union_neighbors(union_bitset, v);
    }

    void union_neighbors(std::set<vertex>& union_set, vertex v) const override
    {
        g_->union_neighbors(union_set, v);
    }

    void diff_neighbors(std::vector<vertex>& diff_vector, vertex v) const override
    {
        g_->diff_neighbors(diff_vector, v);
    }

    void diff_neighbors(DynamicBitset& diff_bitset, vertex v) const override
    {
        g_->diff_neighbors(diff_bitset, v);
    }

    void diff_neighbors(std::set<vertex>& diff_set, vertex v) const override
    {
        g_->diff_neighbors(diff_set, v);
    }

    [[nodiscard]] std::unique_ptr<Graph> clone(bool complementary) const override
    {
        return std::make_unique<WeightedGraph>(g_->clone(complementary),
                                               std::make_unique<WeightedVertices>(*vertex_weights_),
                                               std::make_unique<WeightedEdges>(*edge_weights_));
    }

    void truncate(gint nb_vertices_max) override
    {
        g_->truncate(nb_vertices_max);
    }

    [[nodiscard]] weight_v get_vertex_weight(vertex v) const
    {
        return vertex_weights_->get(g_, v);
    }

    [[nodiscard]] weight_e get_edge_weight(vertex v1, vertex v2) const
    {
        return edge_weights_->get(g_, v1, v2);
    }

    [[nodiscard]] const std::unique_ptr<WeightedVertices>& get_vertex_weights() const
    {
        return vertex_weights_;
    }

    [[nodiscard]] const std::unique_ptr<WeightedEdges>& get_edge_weights() const
    {
        return edge_weights_;
    }

   protected:
    void reset([[maybe_unused]] gint nb_vertices) override
    {
        std::cerr << "Reset is not supported for WeightedGraph\n";
    }

    void add_edge([[maybe_unused]] vertex v1, [[maybe_unused]] vertex v2) override
    {
        std::cerr << "Add edge is not supported for WeightedGraph\n";
    }
};

};  // namespace solverco::utils::graph
