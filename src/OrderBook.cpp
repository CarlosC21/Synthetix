#include "Synthetix/OrderBook.hpp"
#include <algorithm>

namespace synthetix {

template<typename TSide>
void OrderBook::match(Order& incoming, TSide& oppositeSide, std::vector<TradeReport>& trades) {
    // Vectors are contiguous; we start matching from the front (Best Price)
    auto it = oppositeSide.begin();

    while (incoming.quantity > 0 && it != oppositeSide.end()) {
        Price bestPrice = it->first;

        // Check for Price Improvement
        if ((incoming.side == Side::Buy && incoming.price < bestPrice) ||
            (incoming.side == Side::Sell && incoming.price > bestPrice)) {
            break;
        }

        LimitLevel& level = it->second;
        level.fill(incoming, trades);

        if (level.isEmpty()) {
            it = oppositeSide.erase(it); // Note: Shifting elements in vector
        } else {
            break; 
        }
    }
}

LimitLevel& OrderBook::getOrCreateLevel(Price price, Side side) {
    auto& sideRef = (side == Side::Buy) ? bids_ : asks_;
    
    // Custom comparators to maintain sorting
    auto it = (side == Side::Buy) 
        ? std::lower_bound(sideRef.begin(), sideRef.end(), price, [](const LevelPair& a, Price p) { return a.first > p; })
        : std::lower_bound(sideRef.begin(), sideRef.end(), price, [](const LevelPair& a, Price p) { return a.first < p; });

    // If level exists, return it
    if (it != sideRef.end() && it->first == price) {
        return it->second;
    }

    // Otherwise, insert a new level at the binary-searched position
    return sideRef.insert(it, {price, LimitLevel(price)})->second;
}

std::vector<TradeReport> OrderBook::addOrder(Order order) {
    std::vector<TradeReport> trades;

    // 1. Attempt Match
    if (order.side == Side::Buy) {
        match(order, asks_, trades);
    } else {
        match(order, bids_, trades);
    }

    // 2. Add remaining quantity to the book via our flat-map helper
    if (order.quantity > 0) {
        getOrCreateLevel(order.price, order.side).addOrder(order);
    }

    // 3. Update Signals
    auto bBid = getBestBid();
    auto bAsk = getBestAsk();
    Price bP = bBid.value_or(0);
    Price aP = bAsk.value_or(0);
    
    m_signals.updateSignals(bP, getVolumeAtPrice(bP, Side::Buy), 
                            aP, getVolumeAtPrice(aP, Side::Sell));

    return trades;
}

std::optional<Price> OrderBook::getBestBid() const {
    return bids_.empty() ? std::nullopt : std::make_optional(bids_.front().first);
}

std::optional<Price> OrderBook::getBestAsk() const {
    return asks_.empty() ? std::nullopt : std::make_optional(asks_.front().first);
}

Volume OrderBook::getVolumeAtPrice(Price price, Side side) const {
    const auto& sideRef = (side == Side::Buy) ? bids_ : asks_;
    auto it = std::find_if(sideRef.begin(), sideRef.end(), [price](const LevelPair& lp) { return lp.first == price; });
    return (it != sideRef.end()) ? it->second.getTotalVolume() : 0;
}

} // namespace synthetix