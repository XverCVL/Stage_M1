#pragma once

#include <cstdlib>
#include <filesystem>
#include <iostream>
#include <limits>
#include <string>

namespace solverco::utils::benchmark {

namespace fs = std::filesystem;

class BenchmarkVisualizer {
   public:
    static void setup();

    static void generate_boxplot(const std::string& csv, const std::string& out,
                                 const std::string& metric, bool ratio = true,
                                 const std::string& title = "", double min_v = 0.0,
                                 double max_v = std::numeric_limits<double>::max(),
                                 bool zoom = false, bool show = false);

    static void generate_performance_profile(const std::string& csv, const std::string& out,
                                             const std::string& metric,
                                             const std::string& title = "", bool log_x = false,
                                             bool show = false);
};

}  // namespace solverco::utils::benchmark
