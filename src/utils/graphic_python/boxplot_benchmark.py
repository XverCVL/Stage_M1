import csv
import statistics
import matplotlib.pyplot as plt
import argparse

def get_data(input_file):
    data = {}
    solvers = []
    with open(input_file, 'r') as f:
        reader = csv.reader(f)
        headers = next(reader)
        solvers = [h for h in headers[1:] if h]
        for row in reader:
            instance = row[0]
            data[instance] = {solvers[i]: float(row[i+1]) for i in range(len(solvers))}
    return data, solvers

def plot_benchmark(input_file, output_file, metric, zoom, ratio, min_val, max_val, title, show=False):
    data, solvers = get_data(input_file)

    filtered_data = {}
    for inst, values in data.items():
        if all(min_val <= v <= max_val for v in values.values()):
            filtered_data[inst] = values

    if not filtered_data:
        print("No data points remain after filtering.")
        return

    if ratio:
        for inst in filtered_data:
            vals = [filtered_data[inst][s] for s in solvers]
            min_v = min(vals)
            for s in solvers:
                filtered_data[inst][s] /= min_v

    plot_data = {s: [] for s in solvers}
    for inst in filtered_data:
        for s in solvers:
            plot_data[s].append(filtered_data[inst][s])

    if zoom:
        all_vals = [v for s in solvers for v in plot_data[s]]
        q1, q3 = statistics.quantiles(all_vals, n=4)[0], statistics.quantiles(all_vals, n=4)[2]
        iqr = q3 - q1
        low, high = q1 - 1.5 * iqr, q3 + 1.5 * iqr
        for s in solvers:
            plot_data[s] = [v for v in plot_data[s] if low <= v <= high]

    plt.figure(figsize=(10, 6))
    plt.boxplot([plot_data[s] for s in solvers], tick_labels=solvers)
    plt.title(title if title else "Benchmark Boxplot (" + metric + ")")
    plt.xticks(rotation=45, ha='right')
    plt.ylabel("Ratio (" + metric + ")" if ratio else "Value (" + metric + ")")
    plt.tight_layout()
    plt.savefig(output_file)
    if show:
        plt.show()
    plt.close()

if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument("input_file")
    parser.add_argument("output_file")
    parser.add_argument("metric", type=str, default="time", help="Metric to plot (e.g., time, objective)")
    parser.add_argument("--title", type=str, default="", help="Title for the plot")
    parser.add_argument("--zoom", action="store_true")
    parser.add_argument("--ratio", action="store_true")
    parser.add_argument("--show", action="store_true")
    parser.add_argument("--min-val", type=float, default=0.0, help="Minimum threshold for instance inclusion")
    parser.add_argument("--max-val", type=float, default=float('inf'), help="Maximum threshold for instance inclusion")
    args = parser.parse_args()
    plot_benchmark(
        args.input_file,
        args.output_file,
        args.metric,
        args.zoom,
        args.ratio,
        args.min_val,
        args.max_val,
        args.title,
        args.show,
    )
