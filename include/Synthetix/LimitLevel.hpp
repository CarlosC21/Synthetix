#pragma once
#include "Types.hpp"
#include <deque>
#include <vector>
#include <algorithm>

namespace synthetix {

class LimitLevel {
public:
    LimitLevel() : price_(0) {}
    explicit LimitLevel(Price p) : price_(p) {}

    void addOrder(const Order& order) {
        orders_.push_back(order);
    }

    // High-performance fill logic: Price-Time Priority (FIFO)
    void fill(Order& incoming, std::vector<TradeReport>& trades) {
        while (incoming.quantity > 0 && !orders_.empty()) {
            Order& resting = orders_.front();

            // Calculate the fill amount for this specific resting order
            Volume fillQty = std::min(incoming.quantity, resting.quantity);

            if (fillQty > 0) {
                // Generate the trade event
                trades.push_back({resting.id, incoming.id, price_, fillQty});

                incoming.quantity -= fillQty;
                resting.quantity -= fillQty;
            }

            // If the resting order is fully filled, remove it from the time-priority queue
            if (resting.quantity == 0) {
                orders_.pop_front();
            } else {
                // Resting order still has remaining volume; incoming is fully filled
                break;
            }
        }
    }

    Volume getTotalVolume() const {
        Volume total = 0;
        for (const auto& order : orders_) {
            total += order.quantity;
        }
        return total;
    }

    Price getPrice() const { return price_; }
    bool isEmpty() const { return orders_.empty(); }

private:
    Price price_;
    std::deque<Order> orders_; 
};

} // namespace synthetix