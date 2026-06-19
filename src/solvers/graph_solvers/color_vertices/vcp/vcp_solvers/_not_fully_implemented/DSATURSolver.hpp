#pragma once

#include <set>
#include <tuple>
#include <vector>

#include "vcp_solver.hpp"


namespace solverco::graph_solvers::VCP {
struct NodeInfo {
    int sat;
    int deg;
    vertex id;

    bool operator<(const NodeInfo& other) const
    {
        return std::tie(sat, deg, id) > std::tie(other.sat, other.deg, other.id);
    }
};

class DSATURSolver : public VCPSolver {
   protected:
    virtual void initDSATUR(std::vector<std::set<color>>& neighborColors, std::vector<int>& degree,
                            std::set<NodeInfo>& Q);

   public:
    DSATURSolver(Graph* g) : VCPSolver(g) {}

    virtual ~DSATURSolver() = default;


    virtual void solve() override;

    virtual std::string to_string() const override
    {
        return "DSATUR";
    }
};
}  // namespace solverco::graph_solvers::VCP
