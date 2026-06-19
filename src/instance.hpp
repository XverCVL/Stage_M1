#pragma once

#include <memory>
#include <string>

#include "metric.hpp"
#include "solver_def.hpp"

class Instance {
   private:
    std::string name_;

   public:
    explicit Instance(const std::string& name) : name_(name) {}
    virtual ~Instance() = default;

    Instance(const Instance&) = delete;
    Instance& operator=(const Instance&) = delete;
    Instance(Instance&&) = delete;
    Instance& operator=(Instance&&) = delete;

    [[nodiscard]] virtual std::string get_name() const
    {
        return name_;
    }
    virtual void display(std::ostream& stream) const = 0;
};

class InstanceProvider {
   public:
    InstanceProvider() = default;
    virtual ~InstanceProvider() = default;

    InstanceProvider(const InstanceProvider&) = delete;
    InstanceProvider& operator=(const InstanceProvider&) = delete;
    InstanceProvider(InstanceProvider&&) = delete;
    InstanceProvider& operator=(InstanceProvider&&) = delete;

    [[nodiscard]] virtual bool has_next() const = 0;
    virtual std::unique_ptr<Instance> get_next() = 0;
};

class InstanceManager {
   private:
    std::map<std::string, MetricsMap> metrics_instances_;

   protected:
    void add_metrics_instance(const std::string& instance_name, const std::string& metric_name,
                              IMetric* metric)
    {
        metrics_instances_[instance_name][metric_name] = metric;
    }

    void add_all_metrics_instance(const std::string& instance_name, const MetricsMap& metrics)
    {
        metrics_instances_[instance_name] = metrics;
    }

    virtual void load_metrics_instances() = 0;

   public:
    InstanceManager() = default;
    InstanceManager(const InstanceManager&) = delete;
    InstanceManager& operator=(const InstanceManager&) = delete;
    InstanceManager(InstanceManager&&) = delete;
    InstanceManager& operator=(InstanceManager&&) = delete;

    virtual ~InstanceManager()
    {
        for (auto& [_, metrics] : metrics_instances_) {
            for (auto& [_, metric] : metrics) {
                delete metric;
            }
        }
    }

    const std::map<std::string, MetricsMap>& get_metrics_instances()
    {
        if (metrics_instances_.empty()) {
            load_metrics_instances();
        }
        return metrics_instances_;
    }
};
