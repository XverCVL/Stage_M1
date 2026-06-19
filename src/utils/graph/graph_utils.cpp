#include "graph_utils.hpp"

namespace solverco::utils::graph {

void dsatur_partitioning(const Graph& complementary_graph, const DynamicBitset& remaining,
                         const std::vector<uint>& residual_degrees,
                         std::vector<std::vector<vertex>>& clique_coloring_to_fill,
                         bool complementary_for_degrees)
{
    const size_t& n = complementary_graph.nb_vertices();
    if (remaining.none()) {
        return;
    }

    DynamicBitset uncolored = remaining;
    size_t num_uncolored = remaining.count();

    std::vector<uint> degrees = residual_degrees;
    if (complementary_for_degrees) {
        for (auto& deg : degrees) {
            deg = num_uncolored - deg - 1;
        }
    }
    std::vector<DynamicBitset> saturation_colors(n, DynamicBitset(n));

    DynamicBitset targets(n);

    while (num_uncolored > 0) {
        size_t best_v = DynamicBitset::NPOS;
        int max_sat = -1;
        int max_deg = -1;

        uncolored.for_each_bit([&](size_t v) {
            int sat_count = static_cast<int>(saturation_colors[v].count());
            if (sat_count > max_sat ||
                (sat_count == max_sat && static_cast<int>(degrees[v]) > max_deg)) {
                max_sat = sat_count;
                max_deg = static_cast<int>(degrees[v]);
                best_v = v;
            }
        });

        int color = 0;
        while (saturation_colors[best_v].test(color)) {
            color++;
        }

        if (static_cast<size_t>(color) >= clique_coloring_to_fill.size()) {
            clique_coloring_to_fill.resize(color + 1);
        }
        clique_coloring_to_fill[color].push_back(static_cast<vertex>(best_v));

        uncolored.reset(best_v);
        num_uncolored--;

        targets = uncolored;
        complementary_graph.intersect_neighbors(targets, static_cast<vertex>(best_v));

        targets.for_each_bit([&](size_t neighbor) {
            saturation_colors[neighbor].set(color);
            degrees[neighbor]--;
        });
    }
}

unsigned int dsatur(const Graph& g, const DynamicBitset& remaining,
                    const std::vector<uint>& residual_degrees, bool complementary_for_degrees)
{
    const size_t& n = g.nb_vertices();
    if (remaining.none()) {
        return 0;
    }

    DynamicBitset uncolored = remaining;
    size_t num_uncolored = remaining.count();
    int max_color_used = -1;

    std::vector<uint> degrees = residual_degrees;
    if (complementary_for_degrees) {
        for (auto& deg : degrees) {
            deg = num_uncolored - deg - 1;
        }
    }

    std::vector<DynamicBitset> saturation_colors(n, DynamicBitset(n));

    DynamicBitset targets(n);

    while (num_uncolored > 0) {
        size_t best_v = DynamicBitset::NPOS;
        int max_sat = -1;
        int max_deg = -1;

        uncolored.for_each_bit([&](size_t v) {
            int sat_count = static_cast<int>(saturation_colors[v].count());
            if (sat_count > max_sat ||
                (sat_count == max_sat && static_cast<int>(degrees[v]) > max_deg)) {
                max_sat = sat_count;
                max_deg = static_cast<int>(degrees[v]);
                best_v = v;
            }
        });

        int color = 0;
        while (saturation_colors[best_v].test(color)) {
            color++;
        }

        max_color_used = std::max(color, max_color_used);

        uncolored.reset(best_v);
        num_uncolored--;

        targets = uncolored;
        g.intersect_neighbors(targets, static_cast<vertex>(best_v));

        targets.for_each_bit([&](size_t neighbor) {
            saturation_colors[neighbor].set(color);
            degrees[neighbor]--;
        });
    }

    return static_cast<unsigned int>(max_color_used + 1);
}

}  // namespace solverco::utils::graph
