// https://github.com/KarlsruheMIS/KaMIS

// @article{DBLP:journals/heuristics/LammSSSW17,
//   author    = {Sebastian Lamm and
//                Peter Sanders and
//                Christian Schulz and
//                Darren Strash and
//                Renato F. Werneck},
//   title     = {Finding near-optimal independent sets at scale},
//   journal   = {J. Heuristics},
//   volume    = {23},
//   number    = {4},
//   pages     = {207--229},
//   year      = {2017},
//   url       = {https://doi.org/10.1007/s10732-017-9337-x},
//   doi       = {10.1007/s10732-017-9337-x},
//   timestamp = {Fri, 27 Dec 2019 21:13:52 +0100},
//   biburl    = {https://dblp.org/rec/journals/heuristics/LammSSSW17.bib},
//   bibsource = {dblp computer science bibliography, https://dblp.org}
// }

// @article{DBLP:journals/jea/Hespe0S19,
//   author    = {Demian Hespe and
//                Christian Schulz and
//                Darren Strash},
//   title     = {Scalable Kernelization for Maximum Independent Sets},
//   journal   = {{ACM} Journal of Experimental Algorithmics},
//   volume    = {24},
//   number    = {1},
//   pages     = {1.16:1--1.16:22},
//   year      = {2019},
//   url       = {https://doi.org/10.1145/3355502},
//   doi       = {10.1145/3355502},
//   timestamp = {Fri, 27 Mar 2020 08:38:35 +0100},
//   biburl    = {https://dblp.org/rec/journals/jea/Hespe0S19.bib},
//   bibsource = {dblp computer science bibliography, https://dblp.org}
// }

// @inproceedings{DBLP:conf/wea/DahlumLS0SW16,
//   author    = {Jakob Dahlum and
//                Sebastian Lamm and
//                Peter Sanders and
//                Christian Schulz and
//                Darren Strash and
//                Renato F. Werneck},
//   title     = {Accelerating Local Search for the Maximum Independent Set Problem},
//   booktitle = {15th International Symposium on Experimental Algorithms {SEA}},
//   pages     = {118--133},
//   year      = {2016},
//   series    = {Lecture Notes in Computer Science},
//   volume    = {9685},
//   publisher = {Springer},
//   url       = {https://doi.org/10.1007/978-3-319-38851-9\_9}
// }

// @inproceedings{DBLP:conf/alenex/Lamm0SWZ19,
//   author    = {Sebastian Lamm and
//                Christian Schulz and
//                Darren Strash and
//                Robert Williger and
//                Huashuo Zhang},
//   title     = {Exactly Solving the Maximum Weight Independent Set Problem on Large Real-World
//   Graphs}, booktitle = {Proceedings of the Twenty-First Workshop on Algorithm Engineering and
//   Experiments, {ALENEX} 2019}, pages     = {144--158}, year      = {2019}, url       =
//   {https://doi.org/10.1137/1.9781611975499.12}, doi       = {10.1137/1.9781611975499.12},
//   publisher = {{SIAM}},
//   year      = {2019}
// }

// @inproceedings{DBLP:conf/alenex/GellnerLSSZ21,
//   author    = {Alexander Gellner and
//                Sebastian Lamm and
//                Christian Schulz and
//                Darren Strash and
//                Bogd{\'{a}}n Zav{\'{a}}lnij},
//   title     = {Boosting Data Reduction for the Maximum Weight Independent Set Problem
//                Using Increasing Transformations},
//   booktitle = {Proceedings of the 23rd Symposium on Algorithm Engineering and Experiments,
//                {ALENEX} 2021, Virtual Conference, January 10-11, 2021},
//   pages     = {128--142},
//   publisher = {{SIAM}},
//   year      = {2021},
//   url       = {https://doi.org/10.1137/1.9781611976472.10},
//   doi       = {10.1137/1.9781611976472.10},
//   biburl    = {https://dblp.org/rec/conf/alenex/GellnerLSSZ21.bib},
//   bibsource = {dblp computer science bibliography, https://dblp.org}
// }

// @article{DBLP:journals/jgaa/GrossmannLSS24,
//   author    = {Ernestine Gro{\ss}mann and
//                Sebastian Lamm and
//                Christian Schulz and
//                Darren Strash},
//   title     = {Finding Near-Optimal Weight Independent Sets at Scale},
//   journal   = {J. Graph Algorithms Appl.},
//   volume    = {28},
//   number    = {1},
//   pages     = {439--473},
//   year      = {2024},
//   url       = {https://doi.org/10.7155/jgaa.v28i1.2997},
//   doi       = {10.7155/JGAA.V28I1.2997},
//   biburl    = {https://dblp.org/rec/journals/jgaa/GrossmannLSS24.bib},
//   bibsource = {dblp computer science bibliography, https://dblp.org}
// }

#pragma once

#include <string_view>

#include "mis_solver.hpp"
namespace solverco::graph_solvers::mis {

class MISKaMIS : public MISSolver {
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

    explicit MISKaMIS(SolutionManager& solution_manager, Algorithm algorithm,
                      unsigned long time_limit_ = 0)
        : MISSolver(solution_manager), algorithm_(algorithm), time_limit_(time_limit_)
    {}
    explicit MISKaMIS(const InstanceGraph& instance, Algorithm algorithm,
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
