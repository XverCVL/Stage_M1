#pragma once

#include <fstream>
#include <iostream>

#include "graph.hpp"

namespace solverco::utils::graph {

class GraphIO {
   private:
    static void print_vector(const std::vector<vertex>& vec)
    {
        for (auto v : vec) {
            std::cout << " " << v;
        }
        std::cout << "\n";
    }

   public:
    static void display(const Graph& g, std::ostream& f)
    {
        f << "DISPLAY GRAPH\n";
        f << "nb of vertices " << g.nb_vertices() << "\n";
        f << "nb of edges " << g.nb_edges() << "\n";
        for (vertex v1 = 0; v1 < g.nb_vertices(); ++v1) {
            f << "neighbors of " << v1 << " : ";
            for (auto v2 : g.neighbors(v1)) {
                f << v2 << " ";
            }
            f << "\n";
        }
        f << "END DISPLAY GRAPH"
          << "\n";
    }
    static void display_screen(const Graph& g)
    {
        display(g, std::cout);
    }
    static void export_as_dimacs_file(const Graph& g, const std::string& filename)
    {
        std::ofstream file(filename);
        file << "p edge " << g.nb_vertices() << " " << g.nb_edges() << "\n";
        for (vertex v1 = 0; v1 < g.nb_vertices(); ++v1) {
            for (vertex v2 = v1; v2 < g.nb_vertices(); ++v2) {
                if (g.is_edge(v1, v2)) {
                    file << "e " << v1 + 1 << " " << v2 + 1 << "\n";
                }
            }
        }
    }
    static void to_dot(const Graph& g, const std::string& filename)
    {
        std::ofstream file(filename);
        file << "graph {"
             << "\n";
        for (vertex vertex = 0; vertex < g.nb_vertices(); ++vertex) {
            file << vertex << ";"
                 << "\n";
        }
        for (vertex v1 = 0; v1 < g.nb_vertices(); ++v1) {
            for (vertex v2 = v1; v2 < g.nb_vertices(); ++v2) {
                if (g.is_edge(v1, v2)) {
                    file << v1 << " -- " << v2 << ";"
                         << "\n";
                }
            }
        }
        file << "}"
             << "\n";
    }
};

}  // namespace solverco::utils::graph
