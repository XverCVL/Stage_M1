// Philippe Galinier, le 26 octobre 2004

// algorithme TabuCOL

#include "tabucol.hpp"

#include <ctime>
#include <fstream>
#include <iostream>
#include <random>
#include <sstream>
#include <vector>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "dynamic_bitset.hpp"

using namespace std;
using Graph = solverco::utils::graph::Graph;
using Bitset = solverco::utils::containers::DynamicBitset;

void solverco::graph_solvers::vcp::VCPTabuCol::solve()
{
    const auto& graph = get_instance_graph().get_graph();
    const unsigned int max_vtx = graph.nb_vertices();
    const unsigned int max_iter = get_max_iter();
    const double alpha = get_alpha();
    std::minstd_rand generator(get_seed());
    unsigned int max_degree = 0;
    for (unsigned int x = 0; x < max_vtx; x++) {
        unsigned int degree = graph.degree(x);
        if (degree > max_degree) {
            max_degree = degree;
        }
    }
    const unsigned int max_col = max_degree + 1;

    unsigned int* color = new unsigned int[max_vtx]{};
    unsigned int* gamma_ = new unsigned int[max_vtx * max_col]{};
    unsigned int* tabuTenure = new unsigned int[max_vtx * max_col]{};
    Bitset conflicted(max_vtx);

    int f = 0;
    for (vertex x = 0; x < max_vtx; x++) {
        for (vertex y : graph.neighbors(x)) {
            if (x < y) {
                gamma_[x * max_col + color[y]]++;
                gamma_[y * max_col + color[x]]++;
                if (color[x] == color[y]) {
                    f++;
                }
            }
        }
    }


    int f_best = f;

    for (vertex x = 0; x < max_vtx; x++) {
        if (gamma_[x * max_col + color[x]] > 0) {
            conflicted.set(x);
        }
    }

    for (size_t iter = 1; iter < max_iter && f > 0; iter++) {
        int best_x = -1;
        int best_v = -1;
        int nbcfl = 0;

        int best_delta = INT32_MAX;
        int trouve = 0;
        nbcfl = static_cast<int>(conflicted.count());

        best_x = -1;
        best_v = -1;

        size_t x_idx = conflicted.find_first();

        while (x_idx != Bitset::NPOS) {
            const unsigned int x = static_cast<int>(x_idx);
            const unsigned int x_offset = x * max_col;
            const unsigned int x_color = color[x];
            const unsigned int* g_row = &gamma_[x_offset];
            const unsigned int x_conflicts = g_row[x_color];
            const unsigned int* tt_row = &tabuTenure[x_offset];

            bool short_circuit = false;

            for (size_t v = 0; v < max_col; v++) {
                if (v == x_color)
                    continue;

                const int delta = g_row[v] - x_conflicts;

                if (tt_row[v] > iter && (f + delta >= f_best))
                    continue;

                if (delta < best_delta) {
                    best_delta = delta;
                    best_x = x;
                    best_v = v;
                    trouve = 1;

                    if (delta == -x_conflicts) {
                        short_circuit = true;
                        break;
                    }
                } else if (delta == best_delta) {
                    trouve++;
                    if (generator() % trouve == 0) {
                        best_x = x;
                        best_v = v;
                    }
                }
            }

            if (short_circuit) {
                break;
            }

            x_idx = conflicted.find_next(x_idx);
        }

        if (best_x == -1) {
            break;
        }

        const int best_x_offset = best_x * max_col;
        const int old_value = color[best_x];
        const int index = best_x_offset + old_value;

        f += gamma_[best_x_offset + best_v] - gamma_[index];

        for (vertex y : graph.neighbors(best_x)) {
            const unsigned int offset = y * max_col;
            const int y_color = color[y];

            gamma_[offset + old_value]--;
            gamma_[offset + best_v]++;

            if (y_color == best_v) {
                if (gamma_[offset + best_v] == 1) {
                    conflicted.set(y);
                }
            } else if (y_color == old_value) {
                if (gamma_[offset + old_value] == 0) {
                    conflicted.reset(y);
                }
            }
        }

        color[best_x] = best_v;

        if (gamma_[best_x_offset + best_v] == 0) {
            conflicted.reset(best_x);
        } else {
            conflicted.set(best_x);
        }

        const int res = (generator() % 10) + (int)(alpha * nbcfl);
        tabuTenure[index] = iter + res;

        if (f < f_best)
            f_best = f;
    }

    std::vector<vertex> vertex_colors;
    vertex_colors.reserve(max_vtx);

    for (vertex i = 0; i < max_vtx; ++i) {
        vertex_colors.push_back(color[i] + 1);
    }

    auto* solution = new SolutionVCP(get_solution_manager().get_instance(), vertex_colors);
    if (solution->check()) {
        get_solution_manager().insert_solution(solution);
    } else {
        delete solution;
    }

    delete[] color;
    delete[] gamma_;
    delete[] tabuTenure;
}
