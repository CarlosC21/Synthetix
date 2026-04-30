#pragma once

#include <vector>
#include <optional>
#include <algorithm>
#include "Types.hpp"
#include "LimitLevel.hpp"
#include "Signals.hpp"

namespace synthetix {

class OrderBook {
public:
    /**
     * Phase 5: Cache-Friendly Refactor.
     * Replacing std::map with sorted vectors of pairs.
     */
    using LevelPair = std::pair<Price, LimitLevel>;
    using BookSide = std::vector<LevelPair>;

    OrderBook() {
        bids_.reserve(100); // Pre-allocate to reduce heap churn
        asks_.reserve(100);
    }

    std::vector<TradeReport> addOrder(Order order);
    
    std::optional<Price> getBestBid() const;
    std::optional<Price> getBestAsk() const;
    Volume getVolumeAtPrice(Price price, Side side) const;

    const SignalEngine& getSignals() const { return m_signals; }

private:
    BookSide bids_; // Sorted High to Low
    BookSide asks_; // Sorted Low to High
    SignalEngine m_signals;

    // Helper to maintain vector sorting
    LimitLevel& getOrCreateLevel(Price price, Side side);

    template<typename TSide>
    void match(Order& incoming, TSide& oppositeSide, std::vector<TradeReport>& trades);
};

} // namespace synthetix