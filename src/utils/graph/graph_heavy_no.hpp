#pragma once

#include <memory>
#include <vector>

#include "graph_list_adj_no.hpp"

namespace solverco::utils::graph {

class GraphHeavyNO : public GraphListAdjNO {
   public:
    explicit GraphHeavyNO(const std::string& filename);
    GraphHeavyNO(gint nb_vertices, double proba_edge, gint seed);

    GraphHeavyNO() = default;
    ~GraphHeavyNO() override = default;

    GraphHeavyNO(const GraphHeavyNO& other) = default;
    GraphHeavyNO& operator=(const GraphHeavyNO& other) = default;
    GraphHeavyNO(GraphHeavyNO&& other) noexcept = default;
    GraphHeavyNO& operator=(GraphHeavyNO&& other) noexcept = default;

    [[nodiscard]] bool is_edge(vertex v1, vertex v2) const final
    {
        return adj_matrix_[v1][v2];
    }

    void intersect_neighbors(std::vector<vertex>& vect, vertex v1) const final;
    void intersect_neighbors(DynamicBitset& bitset, vertex v1) const final
    {
        bitset &= adj_matrix_[v1];
    }
    void intersect_neighbors(std::set<vertex>& s, vertex v1) const final;

    void union_neighbors(std::vector<vertex>& vect, vertex v1) const final
    {
        GraphListAdjNO::union_neighbors(vect, v1);
    }
    void union_neighbors(DynamicBitset& bitset, vertex v1) const final
    {
        bitset |= adj_matrix_[v1];
    }
    void union_neighbors(std::set<vertex>& s, vertex v1) const final
    {
        GraphListAdjNO::union_neighbors(s, v1);
    }

    void diff_neighbors(std::vector<vertex>& vect, vertex v1) const final;
    void diff_neighbors(DynamicBitset& bitset, vertex v1) const final
    {
        bitset.and_not(adj_matrix_[v1]);
    }
    void diff_neighbors(std::set<vertex>& s, vertex v1) const final;

    void truncate(gint nb_vertices_max) override;

    [[nodiscard]] std::unique_ptr<Graph> clone(bool complementary = false) const override;

    [[nodiscard]] const std::vector<DynamicBitset>& get_adj_matrix() const
    {
        return adj_matrix_;
    }

   protected:
    void reset(gint nb_vertices) override;
    void add_edge(vertex v1, vertex v2) override;

   private:
    std::vector<DynamicBitset> adj_matrix_;
};

}  // namespace solverco::utils::graph
