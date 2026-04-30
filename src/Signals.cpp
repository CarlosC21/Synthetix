#include "Synthetix/Signals.hpp"

namespace synthetix {

void SignalEngine::updateSignals(Price bidPrice, Volume bidVol, Price askPrice, Volume askVol) {
    // 1. Calculate Spread
    m_currentSignals.spread = static_cast<double>(askPrice - bidPrice);

    // 2. Calculate Micro-price
    computeMicroPrice(bidPrice, bidVol, askPrice, askVol);

    // 3. Calculate OFI
    computeOFI(bidPrice, bidVol, askPrice, askVol);

    // 4. Update state for next tick
    m_prevBidPrice = bidPrice;
    m_prevBidVol = bidVol;
    m_prevAskPrice = askPrice;
    m_prevAskVol = askVol;
}

void SignalEngine::computeMicroPrice(Price bP, Volume bV, Price aP, Volume aV) {
    const double totalVol = static_cast<double>(bV + aV);
    if (totalVol > 0.0) {
        m_currentSignals.microPrice = (static_cast<double>(bP) * aV + static_cast<double>(aP) * bV) / totalVol;
    } else {
        m_currentSignals.microPrice = 0.0;
    }
}

void SignalEngine::computeOFI(Price bP, Volume bV, Price aP, Volume aV) {
    double deltaBid = 0.0;
    double deltaAsk = 0.0;

    // Bid pressure logic
    if (bP > m_prevBidPrice) deltaBid = static_cast<double>(bV);
    else if (bP < m_prevBidPrice) deltaBid = -static_cast<double>(m_prevBidVol);
    else deltaBid = static_cast<double>(bV) - static_cast<double>(m_prevBidVol);

    // Ask pressure logic (inverse of bid)
    if (aP < m_prevAskPrice) deltaAsk = static_cast<double>(aV);
    else if (aP > m_prevAskPrice) deltaAsk = -static_cast<double>(m_prevAskVol);
    else deltaAsk = static_cast<double>(aV) - static_cast<double>(m_prevAskVol);

    m_currentSignals.ofi = deltaBid - deltaAsk;
}

std::optional<double> SignalEngine::getMicroPrice() const {
    return (m_currentSignals.microPrice > 0.0) ? std::make_optional(m_currentSignals.microPrice) : std::nullopt;
}

} // namespace synthetix