#pragma once

#include <iostream>
#include <memory>
#include <set>
#include <string>
#include <vector>

#include "../containers/dynamic_bitset.hpp"
#include "graph_defs.hpp"

namespace solverco::utils::graph {

using solverco::utils::containers::DynamicBitset;

class Graph {
   public:
    virtual ~Graph() = default;

    [[nodiscard]] virtual gint nb_vertices() const = 0;

    [[nodiscard]] virtual gint nb_edges() const = 0;

    [[nodiscard]] virtual bool is_edge(vertex v1, vertex v2) const = 0;

    [[nodiscard]] virtual gint degree(vertex v) const = 0;

    [[nodiscard]] virtual const std::vector<vertex>& neighbors(vertex v) const = 0;

    virtual void get_neighbors(vertex v, std::vector<vertex>& neighbors) const = 0;

    virtual void intersect_neighbors(std::vector<vertex>& intersect_vector, vertex v) const = 0;

    virtual void intersect_neighbors(DynamicBitset& intersect_bitset, vertex v) const = 0;

    virtual void intersect_neighbors(std::set<vertex>& intersect_set, vertex v) const = 0;

    virtual void union_neighbors(std::vector<vertex>& union_vector, vertex v) const = 0;

    virtual void union_neighbors(DynamicBitset& union_bitset, vertex v) const = 0;

    virtual void union_neighbors(std::set<vertex>& union_set, vertex v) const = 0;

    virtual void diff_neighbors(std::vector<vertex>& diff_vector, vertex v) const = 0;

    virtual void diff_neighbors(DynamicBitset& diff_bitset, vertex v) const = 0;

    virtual void diff_neighbors(std::set<vertex>& diff_set, vertex v) const = 0;

    [[nodiscard]] virtual std::unique_ptr<Graph> clone(bool complementary) const = 0;

    virtual void truncate(gint nb_vertices_max) = 0;

   protected:
    Graph() = default;

    Graph(const Graph&) = default;
    Graph& operator=(const Graph&) = default;
    Graph(Graph&&) = default;
    Graph& operator=(Graph&&) = default;

    virtual void read_dimacs_file(const std::string& filename, Graph& graph) const;

    virtual void reset(gint nb_vertices) = 0;

    virtual void add_edge(vertex v1, vertex v2) = 0;

    void generate_random(gint nb_vertices, double proba_edges, gint seed);
};

}  // namespace solverco::utils::graph
