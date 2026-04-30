#pragma once

#include "Types.hpp"
#include <string>

namespace synthetix {

struct RiskConfig {
    Volume maxOrderSize{1000};      // Single order limit
    Price  maxOrderValue{100000};   // Total dollar/tick value
    int32_t maxNetPosition{5000};   // Max shares we can hold (+/-)
    double maxPriceDeviation{0.05}; // 5% deviation from mid-price
};

enum class RiskStatus {
    Safe,
    ExceedsOrderSize,
    ExceedsPositionLimit,
    PriceTooFarFromFairValue,
    InvalidOrder
};

class RiskController {
public:
    explicit RiskController(RiskConfig config) : m_config(config) {}

    /**
     * Validates an order before it is sent to the matching engine.
     */
    RiskStatus validateOrder(const Order& order, double currentFairValue);

    /**
     * Updates the internal position tracker after a trade is confirmed.
     */
    void updatePosition(Side side, Volume quantity);

    // Getters
    int32_t getNetPosition() const { return m_netPosition; }

private:
    RiskConfig m_config;
    int32_t m_netPosition{0}; // Positive for Long, Negative for Short
};

} // namespace synthetix