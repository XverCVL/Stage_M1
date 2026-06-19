#include "graph.hpp"

#include <algorithm>
#include <fstream>
#include <iostream>
#include <numeric>
#include <random>
#include <stdexcept>

namespace solverco::utils::graph {

void Graph::read_dimacs_file(const std::string& filename, Graph& graph) const
{
    std::ifstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Error: could not open file " + filename);
    }

    vertex nb_v = 0;
    gint nb_e_expected = 0;
    gint nb_e_actual = 0;
    vertex v1 = 0;
    vertex v2 = 0;

    std::string tag;
    while (file >> tag) {
        if (tag == "c") {
            // Skip comments
            std::string comment;
            std::getline(file, comment);
        } else if (tag == "p") {
            std::string format;
            file >> format >> nb_v >> nb_e_expected;
            graph.reset(nb_v);
        } else if (tag == "e") {
            file >> v1 >> v2;
            // DIMACS is 1-based indexing, we convert to 0-based
            --v1;
            --v2;

            if (!graph.is_edge(v1, v2)) {
                graph.add_edge(v1, v2);
                nb_e_actual++;
            }
        } else {
            // Handle unexpected tags or move to next line
            std::string junk;
            std::getline(file, junk);
        }
    }
}

void Graph::generate_random(gint nb_vertices, double proba_edges, gint seed)  // NOLINT
{
    constexpr gint PROBABILITY_SCALE = 100;
    std::mt19937 generator(seed);
    std::uniform_int_distribution<vertex> distribution(0, PROBABILITY_SCALE);

    gint proba = static_cast<gint>(PROBABILITY_SCALE * proba_edges);
    proba = std::min(proba, static_cast<gint>(PROBABILITY_SCALE));

    reset(nb_vertices);
    for (vertex v1 = 0; v1 < nb_vertices; ++v1) {
        for (vertex v2 = v1 + 1; v2 < nb_vertices; ++v2) {
            if (distribution(generator) < proba) {
                add_edge(v1, v2);
            }
        }
    }
}

}  // namespace solverco::utils::graph
