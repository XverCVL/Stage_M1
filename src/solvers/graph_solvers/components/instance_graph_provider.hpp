#pragma once

#include "instance_graph.hpp"

namespace solverco::graph_solvers {

class InstanceGraphProvider : public InstanceProvider {};

class InstanceGraphProviderFile : public InstanceGraphProvider {
   private:
    std::vector<std::string> filenames_;
    bool complementary_;
    uint trunc_ = 0;
    size_t current_index_ = 0;

   public:
    explicit InstanceGraphProviderFile(const std::vector<std::string>& instance_names,
                                       bool complementary = false, uint trunc = 0)
        : filenames_(instance_names), complementary_(complementary), trunc_(trunc)
    {}

    [[nodiscard]] bool has_next() const override
    {
        return current_index_ < filenames_.size();
    }

    std::unique_ptr<Instance> get_next() override
    {
        if (!has_next()) {
            throw std::out_of_range("No more instances available");
        }
        return std::make_unique<InstanceGraph>(filenames_[current_index_++], complementary_,
                                               trunc_);
    }
};

class InstanceGraphProviderAlea : public InstanceGraphProvider {
   private:
    inline static uint id = 0;

    uint current_index_ = 0;
    uint nb_;
    uint nb_vertices_;
    double proba_edge_;

   public:
    InstanceGraphProviderAlea(uint nb, uint nb_vertices, double proba_edge)
        : nb_(nb), nb_vertices_(nb_vertices), proba_edge_(proba_edge)
    {}

    [[nodiscard]] bool has_next() const override
    {
        return current_index_ < nb_;
    }

    std::unique_ptr<Instance> get_next() override
    {
        if (!has_next()) {
            throw std::out_of_range("No more instances available");
        }
        uint current_id = id++;
        return std::make_unique<InstanceGraph>("graph_alea_" + std::to_string(current_id),
                                               nb_vertices_, proba_edge_, current_id);
    }
};

};  // namespace solverco::graph_solvers
