#pragma once

#include <memory>

#include "instance.hpp"

class Solution {
   private:
    const Instance& instance_;

   public:
    explicit Solution(const Instance& instance) : instance_(instance) {}
    virtual ~Solution() = default;

    Solution(const Solution&) = delete;
    Solution& operator=(const Solution&) = delete;
    Solution(Solution&&) = delete;
    Solution& operator=(Solution&&) = delete;

    [[nodiscard]] virtual bool check() const = 0;
    [[nodiscard]] virtual typeOpt check_cost() = 0;
    [[nodiscard]] virtual typeOpt get_value() const = 0;
    [[nodiscard]] virtual bool equal(Solution* s) const = 0;

    [[nodiscard]] virtual std::unique_ptr<Solution> clone() const = 0;

    virtual void display(std::ostream& stream) const = 0;

    [[nodiscard]] const Instance& get_instance() const
    {
        return instance_;
    }
};
