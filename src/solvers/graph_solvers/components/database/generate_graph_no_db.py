import os
import json
import math
import statistics
from pathlib import Path

SCRIPT_DIR = Path(__file__).resolve().parent

RELATIVE_INSTANCE_PATH = "../../../../../instances/graph_instances/"

TARGET_FOLDER = (SCRIPT_DIR / RELATIVE_INSTANCE_PATH).resolve()

def get_metrics(v, e, degrees):
    if v <= 1:
        return {
            "nb_edges": 0, "density": 0.0, "min_degree": 0, "max_degree": 0,
            "avg_degree": 0.0, "variance_degree": 0.0, "stddev_degree": 0.0, "cv_degree": 0.0
        }

    all_degrees = list(degrees.values())
    avg_deg = (2 * e) / v
    var_deg = statistics.variance(all_degrees) if len(all_degrees) > 1 else 0.0
    std_dev = math.sqrt(var_deg)

    return {
        "nb_edges": e,
        "density": round((2 * e) / (v * (v - 1)), 6),
        "min_degree": min(all_degrees),
        "max_degree": max(all_degrees),
        "avg_degree": round(avg_deg, 3),
        "variance_degree": round(var_deg, 3),
        "stddev_degree": round(std_dev, 3),
        "cv_degree": round(std_dev / avg_deg, 3) if avg_deg > 0 else 0.0
    }

def analyze_dimacs(filepath):
    v_declared = 0
    e_declared = 0
    degrees = {}
    seen_edges = set()
    duplicate_count = 0
    actual_e_count = 0

    with open(filepath, 'r') as f:
        for line in f:
            parts = line.split()
            if not parts: continue

            if parts[0] == 'p':
                v_declared = int(parts[2])
                e_declared = int(parts[3])
                degrees = {i: 0 for i in range(1, v_declared + 1)}
            elif parts[0] == 'e':
                u, v = int(parts[1]), int(parts[2])

                edge = tuple(sorted((u, v)))

                if edge in seen_edges:
                    duplicate_count += 1
                    continue

                seen_edges.add(edge)
                degrees[u] += 1
                degrees[v] += 1
                actual_e_count += 1

    filename_raw = os.path.splitext(os.path.basename(filepath))[0]

    if duplicate_count > 0:
        print(f"! Info: {filename_raw} | {duplicate_count} duplicate(s) ignored.")
    if actual_e_count != e_declared:
        print(f"! Warning: {filename_raw} | Declared: {e_declared} vs Actual: {actual_e_count}")

    original = get_metrics(v_declared, actual_e_count, degrees)

    v_comp = v_declared
    e_comp = (v_declared * (v_declared - 1) // 2) - actual_e_count
    comp_degrees = {n: (v_declared - 1) - d for n, d in degrees.items()}
    complementary = get_metrics(v_comp, e_comp, comp_degrees)

    return {
        "name": filename_raw,
        "nb_vertices": v_declared,
        "nb_edges": original["nb_edges"],
        "density": original["density"],
        "min_degree": original["min_degree"],
        "max_degree": original["max_degree"],
        "avg_degree": original["avg_degree"],
        "variance_degree": original["variance_degree"],
        "stddev_degree": original["stddev_degree"],
        "cv_degree": original["cv_degree"],
        "complementary": complementary
    }

def run(folder):
    if not os.path.exists(folder):
        print(f"Error: The folder '{folder}' does not exist.")
        return

    instances = [analyze_dimacs(os.path.join(folder, f))
                 for f in os.listdir(folder) if f.endswith(".graph")]

    instances.sort(key=lambda x: x["name"].lower())

    output_path = SCRIPT_DIR / 'graph_no_db.json'
    with open(output_path, 'w') as out:
        json.dump({"instances": instances}, out, indent=4)
    print(f"\nDone. JSON generated: {output_path}")

if __name__ == "__main__":
    run(TARGET_FOLDER)
