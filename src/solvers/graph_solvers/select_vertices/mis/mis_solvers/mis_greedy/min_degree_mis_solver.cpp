#include "min_degree_mis_solver.hpp"

#include <algorithm>
#include <random>
#include <vector>

namespace solverco::graph_solvers::mis {

void MinDegreeMISSolver::solve()
{
    const auto& graph = get_graph();

    size_t num_vertices = graph.nb_vertices();
    std::vector<vertex> all_vertices(num_vertices);
    std::iota(all_vertices.begin(), all_vertices.end(), 0);

    std::vector<int> dynamic_degrees(num_vertices);
    std::vector<bool> is_removed(num_vertices, false);
    std::vector<vertex> independent_set;

    std::random_device rd;
    std::mt19937 gen(rd());

    for (vertex v : all_vertices) {
        dynamic_degrees[v] = static_cast<int>(graph.degree(v));
    }

    int remaining_count = static_cast<int>(num_vertices);

    while (remaining_count > 0) {
        int min_deg = std::numeric_limits<int>::max();
        std::vector<vertex> candidates;

        for (vertex v : all_vertices) {
            if (!is_removed[v]) {
                if (dynamic_degrees[v] < min_deg) {
                    min_deg = dynamic_degrees[v];
                    candidates.clear();
                    candidates.push_back(v);
                } else if (dynamic_degrees[v] == min_deg) {
                    candidates.push_back(v);
                }
            }
        }

        if (candidates.empty()) {
            break;
        }

        std::uniform_int_distribution<size_t> dist(0, candidates.size() - 1);
        vertex v_min = candidates[dist(gen)];

        independent_set.push_back(v_min);

        std::vector<vertex> targets_to_remove;
        targets_to_remove.push_back(v_min);

        for (vertex neighbor : graph.neighbors(v_min)) {
            if (!is_removed[neighbor]) {
                targets_to_remove.push_back(neighbor);
            }
        }

        for (vertex to_del : targets_to_remove) {
            if (is_removed[to_del]) {
                continue;
            }

            is_removed[to_del] = true;
            remaining_count--;

            for (vertex neighbor_of_del : graph.neighbors(to_del)) {
                if (!is_removed[neighbor_of_del]) {
                    dynamic_degrees[neighbor_of_del]--;
                }
            }
        }
    }

    std::sort(independent_set.begin(), independent_set.end());

    auto* solution = new SolutionMIS(get_solution_manager().get_instance(), independent_set);
    if (solution->check()) {
        get_solution_manager().insert_solution(solution);
    } else {
        delete solution;
    }
}

};  // namespace solverco::graph_solvers::mis
