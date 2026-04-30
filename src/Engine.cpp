#include "Synthetix/Engine.hpp"

namespace synthetix {

Engine::Engine(RiskConfig riskCfg) 
    : m_risk(riskCfg) {}

std::pair<RiskStatus, std::vector<TradeReport>> Engine::submitOrder(const Order& order) {
    // 1. Extract the current "Fair Value" (Micro-price) to inform Risk
    // If the book is empty, we default to 0.0, but the controller handles this.
    double currentFV = m_orderBook.getSignals().getMicroPrice().value_or(0.0);

    // 2. Pre-Trade Risk Gate
    RiskStatus status = m_risk.validateOrder(order, currentFV);
    
    if (status != RiskStatus::Safe) {
        return { status, {} };
    }

    // 3. Execution (The OrderBook handles matching and internal Signal updates)
    std::vector<TradeReport> trades = m_orderBook.addOrder(order);

    // 4. Post-Trade Position Accounting
    // We update our net position based on the actual quantity filled, not just the order size.
    for (const auto& trade : trades) {
        m_risk.updatePosition(order.side, trade.quantity);
    }

    return { RiskStatus::Safe, trades };
}

} // namespace synthetix