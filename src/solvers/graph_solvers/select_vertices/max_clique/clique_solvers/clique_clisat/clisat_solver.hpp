/*
https://github.com/psanse/CliSAT

Parameters :
    time_limit_ = Time limit in seconds

    is_using_ColorSORT_ = Initial ordering, true for COLOR-SORT, false for DEGREE-SORT
        DEGREE-SORT = based on vertex degree
        COLOR-SORT = based on determining an independent set partition

    is_stochastic_ = Heuristic, true for ATMS 0.05s, false for no heuristic
*/

#pragma once

#include "max_clique_solver.hpp"

namespace solverco::graph_solvers::max_clique {

class CliSATSolver : public MaxCliqueSolver {
   private:
    const unsigned long time_limit_;
    const bool is_using_ColorSORT_;
    const bool is_stochastic_;

   public:
    explicit CliSATSolver(SolutionManager& solution_manager, unsigned long time_limit,
                          bool is_using_ColorSORT, bool is_stochastic)
        : MaxCliqueSolver(solution_manager),
          time_limit_(time_limit),
          is_using_ColorSORT_(is_using_ColorSORT),
          is_stochastic_(is_stochastic)
    {}
    explicit CliSATSolver(const InstanceGraph& instance, unsigned long time_limit,
                          bool is_using_ColorSORT, bool is_stochastic,
                          unsigned int max_nb_solutions = 1)
        : MaxCliqueSolver(instance, max_nb_solutions),
          time_limit_(time_limit),
          is_using_ColorSORT_(is_using_ColorSORT),
          is_stochastic_(is_stochastic)
    {}

    [[nodiscard]] bool is_stochastic() const override
    {
        return is_stochastic_;
    }

    void solve() override;

    std::string to_string_ordering() const
    {
        if (is_using_ColorSORT_) {
            return "Color Sort";
        } else {
            return "Degree Sort";
        }
    }

    std::string to_string_stochastic() const
    {
        if (is_stochastic_) {
            return "ATMS 0.05s";
        } else {
            return "No heuristic";
        }
    }

    [[nodiscard]] std::string to_string() const override
    {
        return "CliSAT " + to_string_ordering() + " " + to_string_stochastic() + " Clique Solver";
    }

    unsigned long get_time_limit()
    {
        return time_limit_;
    }

    std::string get_ordering()
    {
        return std::to_string(is_using_ColorSORT_ + 1);
    }

    inline std::filesystem::path get_path_exec()
    {
#ifdef PATH_BIN_CLISAT
        return std::filesystem::path(PATH_BIN_CLISAT);
#else
        return std::filesystem::path(__FILE__).parent_path() /
               "../../../../../bin/CliSAT_2024_07_06";
#endif
    }
};

};  // namespace solverco::graph_solvers::max_clique
