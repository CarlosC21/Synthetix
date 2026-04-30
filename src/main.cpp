#include <iostream>
#include <iomanip>
#include "Synthetix/Engine.hpp"

using namespace synthetix;

int main() {
    // 1. Configure the system (Limits: 1k shares per order, 5k total position, 5% price deviation)
    RiskConfig config{ 1000, 100000, 5000, 0.05 }; 
    Engine engine(config);

    std::cout << "===========================================" << std::endl;
    std::cout << "Synthetix Trading Engine v1.0 Initialized" << std::endl;
    std::cout << "===========================================" << std::endl;

    // 2. Seed the book with liquidity
    engine.submitOrder(Order(1, 100, 500, Side::Buy));  // Bid 500 @ 100
    engine.submitOrder(Order(2, 105, 500, Side::Sell)); // Ask 500 @ 105

    // 3. Inspect Micro-price Signal
    auto signals = engine.getOrderBook().getSignals();
    std::cout << "Initial Fair Value (Micro-price): " << signals.getMicroPrice().value_or(0.0) << std::endl;

    // 4. Test Risk: Attempt a "Fat Finger" price (Order at 130 when FV is ~102.5)
    std::cout << "\nAttempting Order 3 (Price: 130)..." << std::endl;
    Order roguePrice(3, 130, 100, Side::Buy);
    auto [status, trades] = engine.submitOrder(roguePrice);

    if (status == RiskStatus::PriceTooFarFromFairValue) {
        std::cout << ">>> RISK ALERT: Order 3 rejected due to Price Deviation." << std::endl;
    }

    // 5. Test Execution: Cross the spread
    std::cout << "\nAttempting Order 4 (Aggressive Buy at 105)..." << std::endl;
    Order aggressiveBuy(4, 105, 200, Side::Buy);
    auto [status2, trades2] = engine.submitOrder(aggressiveBuy);

    if (status2 == RiskStatus::Safe && !trades2.empty()) {
        std::cout << ">>> EXECUTION SUCCESS: Filled " << trades2[0].quantity << " shares." << std::endl;
        std::cout << "Current Net Position: " << engine.getRisk().getNetPosition() << " shares." << std::endl;
    }

    // 6. Final Signal State
    auto finalSignals = engine.getOrderBook().getSignals();
    std::cout << "\nFinal Micro-price: " << finalSignals.getMicroPrice().value_or(0.0) << std::endl;
    std::cout << "===========================================" << std::endl;

    return 0;
}