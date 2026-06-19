#pragma once

#include <iostream>
#include <map>
#include <memory>

class IMetric {
   public:
    IMetric() = default;
    virtual ~IMetric() = default;

    IMetric(const IMetric&) = delete;
    IMetric& operator=(const IMetric&) = delete;
    IMetric(IMetric&&) = delete;
    IMetric& operator=(IMetric&&) = delete;

    virtual void print(std::ostream& stream) const = 0;
};

template <typename T>
class Metric : public IMetric {
   private:
    T value_;

   public:
    explicit Metric(T value) : IMetric(), value_(value) {}

    [[nodiscard]] T get_value() const
    {
        return value_;
    }

    void print(std::ostream& stream) const override
    {
        stream << value_;
    }
};

using MetricsMap = std::map<std::string, IMetric*>;
