#include <iostream>
#include <vector>

#include "benchmark_manager.hpp"
#include "clique_stablesolver.hpp"
#include "clisat_solver.hpp"
#include "instance_graph_provider.hpp"
#include "min_degree_mis_solver.hpp"
#include "mis_kamis.hpp"
#include "mis_stablesolver.hpp"
#include "tabucol.hpp"
#include "vcp_colpack.hpp"

int main()
{
    unsigned long timeLimit = 0;
    bool is_using_ColorSORT = true;
    bool is_stochastic = true;
    bool use_complementary_graph = true;

    solverco::graph_solvers::InstanceGraph instance("brock200_1");

    SolutionManager solutionManagerClique(instance, "Clique", OptimizationSense::MAXIMIZE, 1);
    SolutionManager solutionManagerStable(instance, "Stable", OptimizationSense::MAXIMIZE, 1);

    using KaMIS = solverco::graph_solvers::mis::MISKaMIS;
    using AlgorithmKaMIS = KaMIS::Algorithm;

    using StableSolver = solverco::graph_solvers::mis::MISStableSolver;
    using AlgorithmStableSolver = StableSolver::Algorithm;

    using StableSolverClique = solverco::graph_solvers::max_clique::CliqueStableSolver;
    using AlgorithmStableSolverClique = StableSolverClique::Algorithm;

    using ColPack = solverco::graph_solvers::vcp::VCPColPack;
    using MethodColPack = ColPack::Method;
    using OrderingColPack = ColPack::Ordering;

    using CliSAT = solverco::graph_solvers::max_clique::CliSATSolver;

    using TabuCOl = solverco::graph_solvers::vcp::VCPTabuCol;

    solverco::graph_solvers::mis::MinDegreeMISSolver minDegree(solutionManagerStable);

    StableSolver gwMinMIS(solutionManagerStable, AlgorithmStableSolver::GWMin);
    StableSolver gwMin2(solutionManagerStable, AlgorithmStableSolver::GWMin2);
    StableSolver gwMax(solutionManagerStable, AlgorithmStableSolver::GWMax, timeLimit);
    StableSolver strong(solutionManagerStable, AlgorithmStableSolver::Strong);
    StableSolver localSearch(solutionManagerStable, AlgorithmStableSolver::LocalSearch, timeLimit);
    StableSolver localSearchRW1(solutionManagerStable, AlgorithmStableSolver::LocalSearchRW1,
                                timeLimit);
    StableSolver localSearchRW2(solutionManagerStable, AlgorithmStableSolver::LocalSearchRW2,
                                timeLimit);
    StableSolver largeNeighborhood(solutionManagerStable,
                                   AlgorithmStableSolver::LargeNeighborhoodSearch, timeLimit);

    KaMIS greedy(solutionManagerStable, AlgorithmKaMIS::GREEDY, timeLimit);
    KaMIS kernel(solutionManagerStable, AlgorithmKaMIS::KERNEL, timeLimit);
    KaMIS reduMIS(solutionManagerStable, AlgorithmKaMIS::ReduMIS, timeLimit);
    KaMIS onlineMIS(solutionManagerStable, AlgorithmKaMIS::OnlineMIS, timeLimit);
    KaMIS mmWIS(solutionManagerStable, AlgorithmKaMIS::MMWIS, timeLimit);
    KaMIS struction(solutionManagerStable, AlgorithmKaMIS::Struction, timeLimit);
    KaMIS weightedBranchReduce(solutionManagerStable, AlgorithmKaMIS::WeightedBranchReduce,
                               timeLimit);
    KaMIS weightedLocalSearch(solutionManagerStable, AlgorithmKaMIS::WeightedLocalSearch,
                              timeLimit);

    CliSAT cliSAT(solutionManagerClique, timeLimit, is_using_ColorSORT, is_stochastic);

    StableSolverClique gwMinClique(solutionManagerClique, AlgorithmStableSolverClique::GWMin);
    StableSolverClique strongClique(solutionManagerClique, AlgorithmStableSolverClique::Strong);
    StableSolverClique localSearchClique(solutionManagerClique,
                                         AlgorithmStableSolverClique::LocalSearch, timeLimit);

    ColPack colPack(solutionManagerStable, OrderingColPack::NATURAL, MethodColPack::DISTANCE_ONE);
    int seed = 0;
    unsigned int max_iter = 100000;
    double alpha = 0.6;
    TabuCOl tabuCol(solutionManagerStable, seed, max_iter, alpha, timeLimit);
    std::vector<SolverCO*> v;

    // // --- MIS ---
    // v.push_back(&minDegree);  // OK

    // v.push_back(&gwMinMIS);           // OK
    // v.push_back(&gwMin2);             // OK
    // v.push_back(&gwMax);              // OK
    // v.push_back(&strong);             // OK
    // v.push_back(&localSearch);        // OK
    // v.push_back(&localSearchRW1);     // OK
    // v.push_back(&localSearchRW2);     // OK
    // v.push_back(&largeNeighborhood);  // OK

    // v.push_back(&greedy);     // OK
    // v.push_back(&kernel);     // OK
    // v.push_back(&reduMIS);    // OK Seg error on comp
    // v.push_back(&onlineMIS);  // OK

    // v.push_back(&mmWIS);                 // OK BIN
    // v.push_back(&struction);             // OK BIN
    // v.push_back(&weightedBranchReduce);  // OK BIN
    // v.push_back(&weightedLocalSearch);   // OK BIN

    // // --- Clique ---
    // v.push_back(&cliSAT);             // OK
    // v.push_back(&gwMinClique);        // OK
    // v.push_back(&strongClique);       // OK
    // v.push_back(&localSearchClique);  // OK

    // --- VCP ---
    // v.push_back(&colPack);  // OK
    // v.push_back(&tabuCol);  // OK

    solverco::utils::benchmark::BenchmarkManager test(v);
    std::vector<std::string> v2;

    std::ifstream file("test.txt");
    std::string fileName;
    while (file >> fileName) {
        // v2.push_back(fileName.substr(0, fileName.size() - 6));
    }

    // v2.push_back("anna");
    // v2.push_back("qg.order100");

    solverco::graph_solvers::InstanceGraphProviderFile t(v2, use_complementary_graph);
    test.run_benchmark(t);
    test.display_all_results_solutions(std::cout);

    // test.export_to_csv("test.csv", "time");
    test.export_to_csv("best_value.csv", "best_value");
    test.export_to_csv("time.csv", "time");
    solverco::utils::benchmark::BenchmarkVisualizer test2;
    test2.generate_boxplot("CliqueComplementary_best_result.csv",
                           "CliqueComplementary_best_result.png", "best_value");
    test2.generate_boxplot("CliqueComplementary_time.csv", "CliqueComplementary_time.png", "time");
}
