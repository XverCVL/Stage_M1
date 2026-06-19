#include "DSATURSolver.hpp"
#include <set>
#include <vector>
#include <algorithm>
#include <tuple>



void DSATURSolver::initDSATUR(std::vector<std::set<color>>& neighborColors,
                            std::vector<int>& degree,
                            std::set<NodeInfo>& Q) {
    int n = graph->nb_vertices();
    for (vertex v = 0; v < static_cast<vertex>(n); ++v) {
        degree[v] = graph->degree(v);
        Q.insert({0, degree[v], v});
    }
}



void DSATURSolver::solve() {
    int n = graph->nb_vertices();
    colors.assign(n, 0);
    nbColorsUsed = 0;

    std::vector<std::set<color>> neighborColors(n);  
    std::vector<int> degree(n);                      
    std::set<NodeInfo> Q;

    initDSATUR(neighborColors, degree, Q);

    std::vector<bool> usedColors(n + 1, false);

    while (!Q.empty()) {
        vertex u = Q.begin()->id;
        Q.erase(Q.begin());

        // std::cout << "\n[DSATUR] Traitement du sommet " << u << std::endl;

        for (vertex v : graph->neighbors(u)) {
            if (colors[v] != 0) {
                usedColors[colors[v]] = true;
            }
        }

        color c = 1;
        while (usedColors[c]) ++c;
        colors[u] = c;
        nbColorsUsed = std::max(nbColorsUsed, static_cast<color>(c));
        // std::cout << " -> Couleur choisie pour " << u << " : " << c << std::endl;

        for (vertex v : graph->neighbors(u)) {
            if (colors[v] != 0) {
                usedColors[colors[v]] = false;
            }
        }

        for (vertex v : graph->neighbors(u)) {
            if (colors[v] == 0) {
                Q.erase({(int)neighborColors[v].size(), degree[v], v});
                neighborColors[v].insert(c);
                degree[v]--; 
                Q.insert({(int)neighborColors[v].size(), degree[v], v});
                // std::cout << "    -> Mise à jour voisin " << v << ": saturation = " << neighborColors[v].size() << ", degré = " << degree[v] << std::endl;
            }
        }
        // std::cout << "[DSATUR] Nb couleurs utilisées : " << nbColorsUsed << std::endl;
    }

    upperBoundOPT = nbColorsUsed;
    // std::cout << "\n[DSATUR] Terminé. Total de couleurs utilisées : " << nbColorsUsed << std::endl;

}
