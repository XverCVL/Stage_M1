#include "vcp_colpack.hpp"

#include <fstream>
#include <iostream>
#include <vector>

#include <fcntl.h>
#include <sys/wait.h>
#include <unistd.h>

#include "ColPackHeaders.h"

void solverco::graph_solvers::vcp::VCPColPack::solve()
{
    const auto& graph = get_instance_graph().get_graph();
    size_t num_vertices = graph.nb_vertices();

    std::vector<std::vector<vertex>> matrix_graph(num_vertices);

    for (size_t i = 0; i < num_vertices; ++i) {
        vertex degree = graph.degree(i);
        matrix_graph[i].reserve(degree + 1);
        matrix_graph[i].push_back(degree);

        std::vector<vertex> neighbors;
        graph.get_neighbors(i, neighbors);
        for (vertex v : neighbors) {
            matrix_graph[i].push_back(v);
        }
    }

    std::vector<unsigned int*> matrix_ptrs(num_vertices);
    for (unsigned int i = 0; i < num_vertices; ++i) {
        matrix_ptrs[i] = matrix_graph[i].data();
    }

    ColPack::GraphColoringInterface colPackGCI(SRC_MEM_ADOLC, matrix_ptrs.data(), num_vertices);

    colPackGCI.Coloring(get_ordering(), get_method());

    std::vector<int> result;
    colPackGCI.GetVertexColors(result);

    std::vector<vertex> vertex_colors(result.begin(), result.end());
    for (vertex& v : vertex_colors) {
        v++;
    }

    auto* solution = new SolutionVCP(get_solution_manager().get_instance(), vertex_colors);
    if (solution->check()) {
        get_solution_manager().insert_solution(solution);
    } else {
        delete solution;
    }
}
