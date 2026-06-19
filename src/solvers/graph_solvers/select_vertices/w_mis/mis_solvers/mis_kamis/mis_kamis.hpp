// https://github.com/KarlsruheMIS/KaMIS

#pragma once

#include <string_view>

#include "mis_solver.hpp"
namespace solverco::graph_solvers::mis {

class W_MISKaMIS : public MISSolver {
   public:
    enum class Algorithm {
        GREEDY,
        KERNEL,
        MMWIS,
        OnlineMIS,
        ReduMIS,
        Struction,
        WeightedBranchReduce,
        WeightedLocalSearch
    };

   private:
    Algorithm algorithm_;
    const unsigned long time_limit_;

   public:
    constexpr std::string_view to_string_algorithm(Algorithm theme) const
    {
        switch (theme) {
            case Algorithm::GREEDY:
                return "Greedy";
            case Algorithm::KERNEL:
                return "Kernel";
            case Algorithm::MMWIS:
                return "MMWIS";
            case Algorithm::OnlineMIS:
                return "OnlineMIS";
            case Algorithm::ReduMIS:
                return "ReduMIS";
            case Algorithm::Struction:
                return "Struction";
            case Algorithm::WeightedBranchReduce:
                return "Weighted Branch Reduce";
            case Algorithm::WeightedLocalSearch:
                return "Weighted Local Search";
        }
        return "unknown";
    }

    explicit W_MISKaMIS(SolutionManager& solution_manager, Algorithm algorithm,
                        unsigned long time_limit_ = 0)
        : MISSolver(solution_manager), algorithm_(algorithm), time_limit_(time_limit_)
    {}
    explicit W_MISKaMIS(const InstanceGraph& instance, Algorithm algorithm,
                        unsigned long time_limit_ = 0, unsigned int max_nb_solutions = 1)
        : MISSolver(instance, max_nb_solutions), algorithm_(algorithm), time_limit_(time_limit_)
    {}

    [[nodiscard]] bool is_stochastic() const override
    {
        return true;
    }

    void solve() override;

    [[nodiscard]] std::string to_string() const override
    {
        return std::string(to_string_algorithm(algorithm_)) + " KaMIS Solver";
    }

    Algorithm get_algorithm()
    {
        return algorithm_;
    }

    unsigned long get_time_limit()
    {
        return time_limit_;
    }

    inline std::filesystem::path get_path_exec()
    {
#ifdef PATH_BIN_KAMIS
        return std::filesystem::path(PATH_BIN_KAMIS);
#else
        return std::filesystem::path(__FILE__).parent_path() / "../../../../../../../bin/";
#endif
    }
};

};  // namespace solverco::graph_solvers::mis
