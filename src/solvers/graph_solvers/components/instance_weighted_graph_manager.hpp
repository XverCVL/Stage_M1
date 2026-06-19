#pragma once

#include <variant>

#include <nlohmann/json.hpp>

#include "instance_weighted_graph_provider.hpp"

namespace solverco::graph_solvers {

class InstanceWeightedGraphManager : public InstanceManager {
   public:
    void load_metrics_instances() override
    {
        std::ifstream file(std::filesystem::path(__FILE__).parent_path() /
                           "database/graph_no_db.json");
        if (!file.is_open()) {
            std::cerr << "Failed to open metrics file: "
                      << (std::filesystem::path(__FILE__).parent_path() /
                          "database/graph_no_db.json")
                      << "\n";
            return;
        }

        nlohmann::json json_data;
        try {
            file >> json_data;

            if (!json_data.contains("instances") || !json_data["instances"].is_array()) {
                return;
            }

            for (const auto& instance_json : json_data["instances"]) {
                if (!instance_json.contains("name")) {
                    continue;
                }

                std::string instance_name = instance_json["name"];

                for (auto it = instance_json.begin(); it != instance_json.end(); ++it) {
                    const std::string& key = it.key();

                    if (key == "name") {
                        continue;
                    }

                    if (it.value().is_number()) {
                        this->add_metrics_instance(instance_name, key,
                                                   new Metric<double>(it.value().get<double>()));
                    } else if (key == "complementary" && it.value().is_object()) {
                        for (const auto& [comp_key, comp_val] : it.value().items()) {
                            if (comp_val.is_number()) {
                                this->add_metrics_instance(
                                    instance_name, "complementary_" + comp_key,
                                    new Metric<double>(comp_val.get<double>()));
                            }
                        }
                    }
                }
            }
        } catch (const nlohmann::json::exception& e) {
            std::cerr << "JSON Error: " << e.what() << "\n";
        }
    }

    InstanceWeightedGraphProvider* get_weighted_graph_provider(
        const std::map<std::string, std::map<std::string, std::variant<uint, double>>>&
            filter_options,
        bool complementary = false, uint trunc = 0)
    {
        const auto& all_metrics = get_metrics_instances();
        std::vector<std::string> filtered_names;

        auto get_numeric_value = [](IMetric* m) -> double {
            if (auto* d = dynamic_cast<Metric<double>*>(m)) {
                return d->get_value();
            }
            if (auto* u = dynamic_cast<Metric<uint>*>(m)) {
                return static_cast<double>(u->get_value());
            }
            return 0.0;
        };

        auto get_variant_value = [](const std::variant<uint, double>& v) -> double {
            return std::visit([](auto&& arg) -> double { return static_cast<double>(arg); }, v);
        };

        for (const auto& [instance_name, metrics_map] : all_metrics) {
            bool keep_instance = true;

            for (const auto& [field, bounds] : filter_options) {
                std::string target_key =
                    (complementary && field != "nb_vertices") ? "complementary_" + field : field;

                auto it = metrics_map.find(target_key);
                if (it == metrics_map.end()) {
                    continue;
                }

                double val = get_numeric_value(it->second);

                if (bounds.count("min") > 0) {
                    if (val < get_variant_value(bounds.at("min"))) {
                        keep_instance = false;
                        break;
                    }
                }

                if (bounds.count("max") > 0) {
                    if (val > get_variant_value(bounds.at("max"))) {
                        keep_instance = false;
                        break;
                    }
                }
            }

            if (keep_instance) {
                filtered_names.push_back(instance_name);
            }
        }

        if (trunc > 0 && filtered_names.size() > trunc) {
            filtered_names.resize(trunc);
        }

        return new InstanceWeightedGraphProviderFile(filtered_names, complementary, trunc);
    }

    InstanceWeightedGraphProvider* get_alea_weighted_graph_provider(uint nb, uint nb_vertices,
                                                                    double proba_edge)
    {
        return new InstanceWeightedGraphProviderAlea(nb, nb_vertices, proba_edge);
    }
};

};  // namespace solverco::graph_solvers
