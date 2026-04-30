#include "Synthetix/RiskController.hpp"
#include <cmath>

namespace synthetix {

RiskStatus RiskController::validateOrder(const Order& order, double currentFairValue) {
    // 1. Fat-Finger: Order Size Check
    if (order.quantity > m_config.maxOrderSize) {
        return RiskStatus::ExceedsOrderSize;
    }

    // 2. Fat-Finger: Order Value Check (Price * Quantity)
    if (static_cast<double>(order.price) * order.quantity > m_config.maxOrderValue) {
        return RiskStatus::ExceedsOrderSize; // Or a specific ExceedsOrderValue status
    }

    // 3. Position Limit Check
    // Calculate what our position WOULD be if this order fully filled
    int32_t direction = (order.side == Side::Buy) ? 1 : -1;
    int32_t potentialPosition = m_netPosition + (direction * static_cast<int32_t>(order.quantity));

    if (std::abs(potentialPosition) > m_config.maxNetPosition) {
        return RiskStatus::ExceedsPositionLimit;
    }

    // 4. Price Deviation Check (Anti-Fat-Finger for Price)
    // Formula: |OrderPrice - FairValue| / FairValue
    if (currentFairValue > 0.0) {
        double deviation = std::abs(static_cast<double>(order.price) - currentFairValue) / currentFairValue;
        if (deviation > m_config.maxPriceDeviation) {
            return RiskStatus::PriceTooFarFromFairValue;
        }
    }

    return RiskStatus::Safe;
}

void RiskController::updatePosition(Side side, Volume quantity) {
    if (side == Side::Buy) {
        m_netPosition += static_cast<int32_t>(quantity);
    } else {
        m_netPosition -= static_cast<int32_t>(quantity);
    }
}

} // namespace synthetix