#pragma once
#include "Types.hpp"
#include <deque>
#include <numeric>

namespace Synthetix {

    class LimitLevel {
    private:
        int32_t price;
        std::deque<Order> orders; // Deque is more cache-friendly than std::list

    public:
        explicit LimitLevel(int32_t p) : price(p) {}

        void addOrder(const Order& order) {
            orders.push_back(order);
        }

        uint32_t totalVolume() const {
            uint32_t volume = 0;
            for (const auto& order : orders) {
                if (order.is_active) {
                    volume += order.quantity;
                }
            }
            return volume;
        }
    };

} // namespace Synthetix