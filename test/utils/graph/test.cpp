#include <algorithm>
#include <cassert>
#include <chrono>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <vector>

#include "graph_utils.hpp"
#include "instance_graph.hpp"

using solverco::graph_solvers::InstanceGraph;
using solverco::utils::containers::DynamicBitset;
using solverco::utils::graph::dsatur;
using solverco::utils::graph::dsatur_clique;
using solverco::utils::graph::Graph;
using solverco::utils::graph::GraphHeavyNO;
using solverco::utils::graph::GraphIO;
using solverco::utils::graph::vertex;
using std::cerr;
using std::cout;
using std::endl;
using std::string;
using std::vector;
using std::chrono::duration;
using std::chrono::high_resolution_clock;

constexpr int DEFAULT_ITERATIONS = 10000000;
constexpr int DEFAULT_DSATUR_ITERATIONS = 1000;
constexpr vertex DEFAULT_TEST_VERTEX = 10;

class TestGraph : public GraphHeavyNO {
   public:
    using GraphHeavyNO::GraphHeavyNO;
};

void benchmark_performance(const Graph& g, const int& iterations = DEFAULT_ITERATIONS)
{
    if (g.nb_vertices() < 2) {
        return;
    }

    const vertex& v1 = DEFAULT_TEST_VERTEX;

    auto start = high_resolution_clock::now();
    size_t checksum_direct = 0;
    for (int i = 0; i < iterations; ++i) {
        const auto& n = g.neighbors(v1);
        for (vertex v : n) {
            checksum_direct += v;
        }
    }
    auto end = high_resolution_clock::now();
    duration<double, std::milli> time_direct = end - start;

    vector<vertex> union_set;
    auto start_union = high_resolution_clock::now();
    for (int i = 0; i < iterations; ++i) {
        union_set.clear();
        g.union_neighbors(union_set, v1);
    }
    auto end_union = high_resolution_clock::now();
    duration<double, std::milli> time_union = end_union - start_union;

    cout << "\n--- Performance Benchmark (" << iterations << " iterations) ---\n";
    cout << "Direct traversal time:    " << time_direct.count() << " ms\n";
    cout << "union_neighbors time:     " << time_union.count() << " ms\n";

    cout << "Ratio Union/Direct:       " << (time_union.count() / time_direct.count()) << "x\n";
}

void benchmark_dsatur(const Graph& g, int iterations = DEFAULT_DSATUR_ITERATIONS)
{
    const size_t& n = g.nb_vertices();
    if (n == 0) {
        return;
    }

    DynamicBitset remaining(n);
    remaining.set_all();

    vector<vector<vertex>> coloring;

    cout << "\n--- DSATUR Clique Benchmark (" << iterations << " iterations) ---\n";

    std::vector<int> degrees(n, 0);
    remaining.for_each_bit([&](size_t v) {
        const auto& ns = g.neighbors(static_cast<vertex>(v));
        for (vertex neighbor : ns) {
            if (remaining[neighbor]) {
                degrees[v]++;
            }
        }
    });


    auto start = high_resolution_clock::now();

    for (int i = 0; i < iterations; ++i) {
        for (auto& clique : coloring) {
            clique.clear();
        }
        dsatur_clique(g, remaining, degrees, coloring);
        // dsatur(g, remaining, degrees);
    }

    auto end = high_resolution_clock::now();
    duration<double, std::milli> total_time = end - start;

    size_t nb_cliques = 0;
    for (const auto& c : coloring) {
        if (!c.empty()) {
            nb_cliques++;
        }
    }

    cout << "Total time:     " << total_time.count() << " ms\n";
    cout << "Avg per call:   " << (total_time.count() / iterations) << " ms\n";
    cout << "Cliques found:  " << nb_cliques << " (dernier run)\n";
    cout << "-----------------------------------------------\n";
}

void test_load_dimacs(const string& file_name)
{
    InstanceGraph instance(file_name);
    try {
        const auto& g = instance.get_graph();
        assert(g.nb_vertices() > 0);
        cout << "[OK] Loaded: " << file_name << " (" << g.nb_vertices() << "v, " << g.nb_edges()
             << "e)\n";
        GraphIO::display_screen(g);
        auto g_complementary = g.clone(true);
        cout << "[OK] Complementary graph: " << g_complementary->nb_vertices() << "v, "
             << g_complementary->nb_edges() << "e)\n";
        GraphIO::display_screen(*g_complementary);
        benchmark_performance(g);
    } catch (const std::exception& e) {
        cerr << "[ERROR] " << e.what() << "\n";
        throw;
    }
}

int main(int argc, char* argv[])
{
    // test_load_dimacs("myciel3");

    constexpr int DSATUR_BENCHMARK_ITERATIONS = 18000;

    try {
        InstanceGraph instance("C125.9");
        const auto& g = instance.get_graph();

        cout << "[OK] Graph loaded for DSATUR test: " << g.nb_vertices() << " vertices.\n";

        benchmark_dsatur(g, DSATUR_BENCHMARK_ITERATIONS);

    } catch (const std::exception& e) {
        cerr << "[ERROR] " << e.what() << "\n";
        return 1;
    }
    return 0;

    return 0;
}
