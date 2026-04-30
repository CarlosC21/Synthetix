#pragma once

#include "OrderBook.hpp"
#include "RiskController.hpp"
#include <vector>
#include <utility>

namespace synthetix {

/**
 * @class Engine
 * The central nervous system of Synthetix.
 * Coordinates Risk validation, OrderBook execution, and Signal generation.
 */
class Engine final {
public:
    explicit Engine(RiskConfig riskCfg);

    /**
     * The primary entry point for trading.
     * Performs pre-trade risk checks, executes in the book, and updates post-trade state.
     * @return A pair containing the RiskStatus and any resulting TradeReports.
     */
    std::pair<RiskStatus, std::vector<TradeReport>> submitOrder(const Order& order);

    // Read-only access for monitoring and UI/Strategy layers
    const OrderBook& getOrderBook() const { return m_orderBook; }
    const RiskController& getRisk() const { return m_risk; }

private:
    OrderBook m_orderBook;
    RiskController m_risk;
};

} // namespace synthetix