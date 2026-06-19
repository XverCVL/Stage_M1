#include "benchmark_visualizer.hpp"

namespace solverco::utils::benchmark {


void BenchmarkVisualizer::setup()
{
    fs::path venv_path = fs::current_path() / "venv";
    fs::path script_dir = fs::path(__FILE__).parent_path().parent_path() / "graphic_python";

    fs::path python_exec = venv_path / "bin" / "python3";

    if (!fs::exists(venv_path)) {
        std::cout << "Initializing local Python environment...\n";
        std::system(("python3 -m venv " + venv_path.string()).c_str());
        std::system((python_exec.string() + " -m pip install -r " +
                     (script_dir / "requirements.txt").string())
                        .c_str());
    }
}

void BenchmarkVisualizer::generate_boxplot(const std::string& csv, const std::string& out,
                                           const std::string& metric, bool ratio,
                                           const std::string& title, double min_v, double max_v,
                                           bool zoom, bool show)
{
    BenchmarkVisualizer::setup();
    fs::path script_dir = fs::path(__FILE__).parent_path().parent_path() / "graphic_python";
    fs::path python_exec = fs::current_path() / "venv" / "bin" / "python3";

    std::string cmd = python_exec.string() + " " + (script_dir / "boxplot_benchmark.py").string() +
                      " " + csv + " " + out + " " + metric + (ratio ? " --ratio" : "") +
                      " --min-val " + std::to_string(min_v) + " --max-val " +
                      std::to_string(max_v) + (zoom ? " --zoom" : "") + (show ? " --show" : "") +
                      (title.empty() ? "" : " --title \"" + title + "\"");

    std::system(cmd.c_str());
    std::cout << "Generated boxplot: " << out << "\n";
}

void BenchmarkVisualizer::generate_performance_profile(const std::string& csv,
                                                       const std::string& out,
                                                       const std::string& metric,
                                                       const std::string& title, bool log_x,
                                                       bool show)
{
    BenchmarkVisualizer::setup();
    fs::path script_dir = fs::path(__FILE__).parent_path().parent_path() / "graphic_python";
    fs::path python_exec = fs::current_path() / "venv" / "bin" / "python3";

    std::string cmd = python_exec.string() + " " +
                      (script_dir / "performance_profile_benchmark.py").string() + " " + csv + " " +
                      out + " " + metric + (log_x ? " --log-x" : "") + (show ? " --show" : "") +
                      (title.empty() ? "" : " --title \"" + title + "\"");

    std::system(cmd.c_str());
    std::cout << "Generated performance profile: " << out << "\n";
}

}  // namespace solverco::utils::benchmark
