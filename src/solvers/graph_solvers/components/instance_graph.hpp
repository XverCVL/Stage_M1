#pragma once

#include <algorithm>
#include <filesystem>
#include <fstream>
#include <variant>

#include "graph_heavy_no.hpp"
#include "graph_io.hpp"
#include "instance.hpp"

namespace solverco::graph_solvers {

using solverco::utils::graph::Graph;
using solverco::utils::graph::GraphHeavyNO;
using solverco::utils::graph::GraphIO;

inline std::filesystem::path get_path_instances_graph()
{
#ifdef PATH_GRAPH_INSTANCES
    return std::filesystem::path(PATH_GRAPH_INSTANCES);
#else
    return std::filesystem::path(__FILE__).parent_path() / "../../../../instances/graph_instances/";
#endif
}

class InstanceGraph : public Instance {
   private:
    Graph* graph_;

   public:
    InstanceGraph() : Instance(""), graph_(new GraphHeavyNO()) {}

    explicit InstanceGraph(const std::string& instance_name, bool complementary = false,
                           uint trunc = 0)
        : Instance(instance_name + ((trunc > 0) ? ("_trunc" + std::to_string(trunc)) : "")),
          graph_(
              new GraphHeavyNO(std::string(get_path_instances_graph()) + instance_name + ".graph"))
    {
        if (complementary) {
            auto* temp = graph_;
            graph_ = dynamic_cast<Graph*>(graph_->clone(true).release());
            delete temp;
        }
        if (trunc > 0) {
            graph_->truncate(trunc);
        }
    };

    InstanceGraph(const std::string& name, uint nb_vertices, double proba_edge, uint seed)
        : Instance(name), graph_(new GraphHeavyNO(nb_vertices, proba_edge, seed))
    {}

    ~InstanceGraph() override
    {
        delete graph_;
    };

    InstanceGraph(const InstanceGraph&) = delete;
    InstanceGraph& operator=(const InstanceGraph&) = delete;
    InstanceGraph(InstanceGraph&&) = delete;
    InstanceGraph& operator=(InstanceGraph&&) = delete;

    void display(std::ostream& stream) const override
    {
        GraphIO::display(*graph_, stream);
    }

    [[nodiscard]] const Graph& get_graph() const
    {
        return *graph_;
    }
};


};  // namespace solverco::graph_solvers
