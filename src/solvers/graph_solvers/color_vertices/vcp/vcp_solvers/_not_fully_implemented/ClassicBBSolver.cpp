#include "ClassicBBSolver.hpp"
#include <algorithm>

ClassicBBSolver::ClassicBBSolver(Graph* graph)
    : VCPSolver(graph), bestNbColors(graph->nb_vertices()) {
    current.assign(nb_sommets, 0);
    bestColoring.assign(nb_sommets, 0);
}
void ClassicBBSolver::solve() {
    start_time = std::chrono::steady_clock::now();  

    branch(0, 0);

    auto end = std::chrono::steady_clock::now();
    std::chrono::duration<double> elapsed = end - start_time;
    setStats(elapsed.count(), bestNbColors);

    colors = bestColoring;
    nbColorsUsed = bestNbColors;
    upperBoundOPT = bestNbColors;
}

void ClassicBBSolver::branch(vertex v, int nbColorsUsed) {
    auto now = std::chrono::steady_clock::now();
    std::chrono::duration<double> elapsed = now - start_time;
    if (elapsed.count() > TIME_LIMIT_SECONDS) return;

    if (nbColorsUsed >= bestNbColors) return;

    if (v == nb_sommets) {
        bestColoring = current;
        bestNbColors = nbColorsUsed;
        std::cout << "[Update] Nouvelle solution avec " << bestNbColors << " couleurs." << std::endl;
        return;
    }

    for (color c = 1; c <= static_cast<color>(nbColorsUsed + 1); ++c) {
        bool ok = true;

        for (vertex u : graph->neighbors(v)) {
            if (u < v && current[u] == c) {
                ok = false;
                break;
            }
        }

        if (ok) {
            current[v] = c;
            branch(v + 1, std::max(nbColorsUsed, (int)c));
            current[v] = 0;
        }
    }
}
