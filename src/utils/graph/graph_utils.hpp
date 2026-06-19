#pragma once

#include <algorithm>
#include <list>
#include <vector>

#include "../containers/dynamic_bitset.hpp"
#include "graph.hpp"

namespace solverco::utils::graph {

using namespace solverco::utils::graph;
using namespace solverco::utils::containers;

void dsatur_partitioning(const Graph &complementary_graph, const DynamicBitset &remaining,
                         const std::vector<uint> &residual_degrees,
                         std::vector<std::vector<vertex>> &clique_coloring_to_fill,
                         bool complementary_for_degrees = false);

unsigned int dsatur(const Graph &g, const DynamicBitset &remaining,
                    const std::vector<uint> &residual_degrees,
                    bool complementary_for_degrees = false);

}  // namespace solverco::utils::graph
