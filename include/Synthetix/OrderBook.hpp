#pragma once
#include <map>
#include <optional>
#include <vector>
#include <functional>
#include "Types.hpp"
#include "LimitLevel.hpp"

namespace synthetix {

class OrderBook {
public:
    using BidMap = std::map<Price, LimitLevel, std::greater<Price>>;
    using AskMap = std::map<Price, LimitLevel, std::less<Price>>;

    // Changed: Returns trades executed by this order
    std::vector<TradeReport> addOrder(Order order);
    
    std::optional<Price> getBestBid() const;
    std::optional<Price> getBestAsk() const;
    Volume getVolumeAtPrice(Price price, Side side) const;

private:
    BidMap bids_;
    AskMap asks_;

    // Template helper to match an incoming order against the opposite side of the book
    // This maintains Price-Time priority.
    template<typename TMap>
    void match(Order& incoming, TMap& oppositeSide, std::vector<TradeReport>& trades);
};

} // namespace synthetix