#pragma once

#include "Types.hpp"
#include <optional>

namespace synthetix {

/**
 * @struct MarketSignals
 * POD for the current alpha state. Using doubles for signals to maintain
 * precision during weighted calculations.
 */
struct MarketSignals {
    double microPrice{0.0};
    double ofi{0.0};
    double spread{0.0};
};

class SignalEngine {
public:
    SignalEngine() = default;

    /**
     * Updates signals based on the current Best Bid and Offer (BBO).
     * @param bidPrice Best bid price
     * @param bidVol Total volume at the best bid
     * @param askPrice Best ask price
     * @param askVol Total volume at the best ask
     */
    void updateSignals(Price bidPrice, Volume bidVol, Price askPrice, Volume askVol);

    // Getters
    [[nodiscard]] std::optional<double> getMicroPrice() const;
    [[nodiscard]] double getOFI() const { return m_currentSignals.ofi; }
    [[nodiscard]] double getSpread() const { return m_currentSignals.spread; }

private:
    MarketSignals m_currentSignals;

    // State tracking for OFI calculation
    Price  m_prevBidPrice{0};
    Volume m_prevBidVol{0};
    Price  m_prevAskPrice{0};
    Volume m_prevAskVol{0};

    void computeMicroPrice(Price bP, Volume bV, Price aP, Volume aV);
    void computeOFI(Price bP, Volume bV, Price aP, Volume aV);
};

} // namespace synthetix