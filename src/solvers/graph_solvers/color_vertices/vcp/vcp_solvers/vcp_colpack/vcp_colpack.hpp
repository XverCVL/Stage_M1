/*
https://github.com/CSCsw/ColPack

Parameters :
    time_limit_ = Time limit in seconds

    ordering_ = Ordering of the vertex
        LARGEST_FIRST = TODO
        SMALLEST_LAST = TODO
        DYNAMIC_LARGEST_FIRST = TODO
        INCIDENCE_DEGREE = TODO
        NATURAL = TODO
        RANDOM = TODO

    method_ = Method to solve the VCP
        DISTANCE_ONE = TODO
        ACYCLIC = TODO
        ACYCLIC_FOR_INDIRECT_RECOVERY = TODO
        STAR = TODO
        RESTRICTED_STAR = TODO
        DISTANCE_TWO = TODO

        IMPLICIT_COVERING__STAR_BICOLORING = TODO
        EXPLICIT_COVERING__STAR_BICOLORING = TODO
        EXPLICIT_COVERING__MODIFIED_STAR_BICOLORING = TODO
        IMPLICIT_COVERING__GREEDY_STAR_BICOLORING = TODO

        COLUMN_PARTIAL_DISTANCE_TWO = TODO
        ROW_PARTIAL_DISTANCE_TWO = TODO

        D1_OMP_GMMP = TODO
        D1_OMP_GM3P = TODO
        D1_OMP_GMMP_LOLF = TODO
        D1_OMP_GM3P_LOLF = TODO

        D2_OMP_GMMP = TODO
        D2_OMP_GM3P = TODO
        D2_OMP_GMMP_LOLF = TODO
        D2_OMP_GM3P_LOLF = TODO

        PD2_OMP_GMMP = TODO
        PD2_OMP_GM3P = TODO
        PD2_OMP_GMMP_LOLF = TODO
        PD2_OMP_GM3P_LOLF = TODO
*/

#pragma once

#include <string_view>
#include <vector>

#include "solution_vcp.hpp"
#include "vcp_solver.hpp"

namespace solverco::graph_solvers::vcp {

class VCPColPack : public VCPSolver {
   public:
    enum class Ordering {
        NATURAL,
        LARGEST_FIRST,
        DYNAMIC_LARGEST_FIRST,
        DISTANCE_TWO_LARGEST_FIRST,
        SMALLEST_LAST,
        DISTANCE_TWO_SMALLEST_LAST,
        INCIDENCE_DEGREE,
        DISTANCE_TWO_INCIDENCE_DEGREE,
        RANDOM
    };

    enum class Method {
        DISTANCE_ONE,
        ACYCLIC,
        ACYCLIC_FOR_INDIRECT_RECOVERY,
        STAR,
        RESTRICTED_STAR,
        DISTANCE_TWO,
    };

   private:
    Ordering ordering_;
    Method method_;
    const unsigned long time_limit_;

   public:
    constexpr std::string_view to_string_method(Method method) const
    {
        switch (method) {
            case Method::DISTANCE_ONE:
                return "DISTANCE_ONE";
            case Method::ACYCLIC:
                return "ACYCLIC";
            case Method::ACYCLIC_FOR_INDIRECT_RECOVERY:
                return "ACYCLIC_FOR_INDIRECT_RECOVERY";
            case Method::STAR:
                return "STAR";
            case Method::RESTRICTED_STAR:
                return "RESTRICTED_STAR";
            case Method::DISTANCE_TWO:
                return "DISTANCE_TWO";
        }
        return "unknown";
    }

    constexpr std::string_view to_string_ordering(Ordering ordering) const
    {
        switch (ordering) {
            case Ordering::NATURAL:
                return "NATURAL";
            case Ordering::LARGEST_FIRST:
                return "LARGEST_FIRST";
            case Ordering::DYNAMIC_LARGEST_FIRST:
                return "DYNAMIC_LARGEST_FIRST";
            case Ordering::DISTANCE_TWO_LARGEST_FIRST:
                return "DISTANCE_TWO_LARGEST_FIRST";
            case Ordering::SMALLEST_LAST:
                return "SMALLEST_LAST";
            case Ordering::DISTANCE_TWO_SMALLEST_LAST:
                return "DISTANCE_TWO_SMALLEST_LAST";
            case Ordering::INCIDENCE_DEGREE:
                return "INCIDENCE_DEGREE";
            case Ordering::DISTANCE_TWO_INCIDENCE_DEGREE:
                return "DISTANCE_TWO_INCIDENCE_DEGREE";
            case Ordering::RANDOM:
                return "RANDOM";
        }
        return "unknown";
    }

    explicit VCPColPack(SolutionManager& solution_manager, Ordering ordering, Method method,
                        unsigned long time_limit_ = 0)
        : VCPSolver(solution_manager),
          ordering_(ordering),
          method_(method),
          time_limit_(time_limit_)
    {}
    explicit VCPColPack(const InstanceGraph& instance, Ordering ordering, Method method,
                        unsigned long time_limit_ = 0, unsigned int max_nb_solutions = 1)
        : VCPSolver(instance, max_nb_solutions),
          ordering_(ordering),
          method_(method),
          time_limit_(time_limit_)
    {}

    [[nodiscard]] bool is_stochastic() const override
    {
        return true;
    }

    void solve() override;

    [[nodiscard]] std::string to_string() const override
    {
        return "Colpack " + get_ordering() + " " + get_method() + " VCP Solver";
    }

    std::string get_method() const
    {
        return std::string(to_string_method(method_));
    }

    std::string get_ordering() const
    {
        return std::string(to_string_ordering(ordering_));
    }

    unsigned long get_time_limit()
    {
        return time_limit_;
    }
};

};  // namespace solverco::graph_solvers::vcp
