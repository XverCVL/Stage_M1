#pragma once

#include "vcp_solver.hpp"

class ClassicBBSolver : public VCPSolver {
   private:
    std::vector<color> current;
    std::vector<color> bestColoring;
    int bestNbColors;
    std::chrono::steady_clock::time_point start_time;
    constexpr static double TIME_LIMIT_SECONDS = 300.0;


    void branch(vertex v, int nbColorsUsed);

   public:
    ClassicBBSolver(Graph* graph);

    void solve() override;
    std::string to_string() const override
    {
        return "ClassicBB";
    }
};
