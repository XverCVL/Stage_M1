#pragma once

#include <filesystem>

#include "graph_heavy_no.hpp"
#include "graph_io.hpp"
#include "instance_graph.hpp"
#include "weighted_graph.hpp"

namespace solverco::graph_solvers {

using solverco::utils::graph::GraphHeavyNO;
using solverco::utils::graph::GraphIO;
using solverco::utils::graph::WeightedGraph;

class InstanceWeightedGraph : public Instance {
   private:
    WeightedGraph* graph_;

   public:
    InstanceWeightedGraph()
        : Instance(""), graph_(new WeightedGraph(std::make_unique<GraphHeavyNO>()))
    {}

    explicit InstanceWeightedGraph(const std::string& instance_name, bool complementary = false,
                                   uint trunc = 0)
        : Instance(instance_name + ((trunc > 0) ? ("_trunc" + std::to_string(trunc)) : "")),
          graph_(new WeightedGraph(std::make_unique<GraphHeavyNO>(
              std::string(get_path_instances_graph()) + instance_name + ".graph")))
    {
        if (complementary) {
            auto* temp = graph_;
            graph_ = dynamic_cast<WeightedGraph*>(graph_->clone(true).release());
            delete temp;
        }
        if (trunc > 0) {
            graph_->truncate(trunc);
        }
    };

    InstanceWeightedGraph(const std::string& name, uint nb_vertices, double proba_edge, uint seed)
        : Instance(name),
          graph_(new WeightedGraph(std::make_unique<GraphHeavyNO>(nb_vertices, proba_edge, seed)))
    {}

    ~InstanceWeightedGraph() override
    {
        delete graph_;
    };

    InstanceWeightedGraph(const InstanceWeightedGraph&) = delete;
    InstanceWeightedGraph& operator=(const InstanceWeightedGraph&) = delete;
    InstanceWeightedGraph(InstanceWeightedGraph&&) = delete;
    InstanceWeightedGraph& operator=(InstanceWeightedGraph&&) = delete;

    void display(std::ostream& stream) const override
    {
        GraphIO::display(*graph_, stream);
    }

    [[nodiscard]] const WeightedGraph& get_graph() const
    {
        return *graph_;
    }
};
};  // namespace solverco::graph_solvers
