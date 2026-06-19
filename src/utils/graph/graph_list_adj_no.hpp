#pragma once

#include <string>
#include <vector>

#include "graph.hpp"

namespace solverco::utils::graph {

class GraphListAdjNO : public Graph {
   public:
    explicit GraphListAdjNO(const std::string& filename);
    GraphListAdjNO(gint nb_vertices, double proba_edge, gint seed);

    GraphListAdjNO() = default;

    GraphListAdjNO(const GraphListAdjNO& other) = default;
    GraphListAdjNO& operator=(const GraphListAdjNO& other) = default;
    GraphListAdjNO(GraphListAdjNO&& other) noexcept = default;
    GraphListAdjNO& operator=(GraphListAdjNO&& other) noexcept = default;

    ~GraphListAdjNO() override = default;

    [[nodiscard]] gint nb_vertices() const override;
    [[nodiscard]] gint nb_edges() const override;

    [[nodiscard]] bool is_edge(vertex v1, vertex v2) const override;
    [[nodiscard]] gint degree(vertex v) const override;
    void get_neighbors(vertex v, std::vector<vertex>& neighbors) const override;
    [[nodiscard]] const std::vector<vertex>& neighbors(vertex v) const override;

    void intersect_neighbors(std::vector<vertex>& vect, vertex v1) const override;
    void intersect_neighbors(DynamicBitset& bitset, vertex v1) const override;
    void intersect_neighbors(std::set<vertex>& s, vertex v1) const override;

    void union_neighbors(std::vector<vertex>& vect, vertex v1) const override;
    void union_neighbors(DynamicBitset& bitset, vertex v1) const override;
    void union_neighbors(std::set<vertex>& s, vertex v1) const override;

    void diff_neighbors(std::vector<vertex>& vect, vertex v1) const override;
    void diff_neighbors(DynamicBitset& bitset, vertex v1) const override;
    void diff_neighbors(std::set<vertex>& s, vertex v1) const override;

    void truncate(gint nb_vertices_max) override;

    [[nodiscard]] std::unique_ptr<Graph> clone(bool complementary = false) const override;

   protected:
    void reset(gint nb_vertices) override;
    void add_edge(vertex v1, vertex v2) override;

   private:
    gint nb_edges_{0};
    std::vector<std::vector<vertex>> neighbors_;
};

}  // namespace solverco::utils::graph
