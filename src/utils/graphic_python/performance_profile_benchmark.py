import argparse
import csv
import math

import matplotlib.pyplot as plt


def get_data(input_file):
    data = {}
    solvers = []
    with open(input_file, "r", newline="") as f:
        reader = csv.reader(f)
        headers = next(reader)
        solvers = [h for h in headers[1:] if h]
        for row in reader:
            if not row:
                continue
            instance = row[0]
            values = {}
            for i, solver in enumerate(solvers):
                if i + 1 >= len(row) or row[i + 1] == "":
                    values = {}
                    break
                values[solver] = float(row[i + 1])
            if values:
                data[instance] = values
    return data, solvers


def compute_ratios(data, solvers):
    ratios = {solver: [] for solver in solvers}

    for _, values in data.items():
        instance_values = [values[solver] for solver in solvers]
        best_value = min(instance_values)

        if best_value < 0:
            raise ValueError("Performance profile requires non-negative values.")

        if best_value == 0:
            if any(value != 0 for value in instance_values):
                raise ValueError(
                    "Performance profile undefined when best value is 0 and others are positive."
                )
            for solver in solvers:
                ratios[solver].append(1.0)
            continue

        for solver in solvers:
            ratios[solver].append(values[solver] / best_value)

    return ratios


def plot_performance_profile(input_file, output_file, metric, title, log_x, show=False):
    data, solvers = get_data(input_file)

    if not data:
        print("No data points available.")
        return

    ratios = compute_ratios(data, solvers)

    plt.figure(figsize=(10, 6))
    max_ratio = 1.0

    for solver in solvers:
        solver_ratios = sorted(ratios[solver])
        if not solver_ratios:
            continue

        max_ratio = max(max_ratio, solver_ratios[-1])
        x_values = [1.0] + solver_ratios
        y_values = [0.0] + [(i + 1) / len(solver_ratios) for i in range(len(solver_ratios))]
        plt.step(x_values, y_values, where="post", label=solver)

    if log_x:
        plt.xscale("log")

    upper_bound = max_ratio * 1.05 if max_ratio > 1.0 else 1.1
    plt.xlim(left=1.0, right=upper_bound)
    plt.ylim(0.0, 1.01)
    plt.xlabel("Facteur de performance τ")
    plt.ylabel("Proportion d'instances")
    plt.title(title if title else f"Performance Profile ({metric})")
    plt.grid(True, which="both", axis="both", linestyle="--", linewidth=0.5, alpha=0.5)
    plt.legend()
    plt.tight_layout()
    plt.savefig(output_file)
    if show:
        plt.show()
    plt.close()


if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument("input_file")
    parser.add_argument("output_file")
    parser.add_argument("metric", type=str, help="Metric to plot (e.g., time, objective)")
    parser.add_argument("--title", type=str, default="", help="Title for the plot")
    parser.add_argument("--log-x", action="store_true", help="Use a logarithmic scale on x axis")
    parser.add_argument("--show", action="store_true", help="Open the matplotlib window")
    args = parser.parse_args()

    plot_performance_profile(
        args.input_file, args.output_file, args.metric, args.title, args.log_x, args.show
    )
