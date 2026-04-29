#include "Synthetix/OrderBook.hpp"

namespace synthetix {

template<typename TMap>
void OrderBook::match(Order& incoming, TMap& oppositeSide, std::vector<TradeReport>& trades) {
    auto it = oppositeSide.begin();

    while (incoming.quantity > 0 && it != oppositeSide.end()) {
        Price bestPrice = it->first;

        if ((incoming.side == Side::Buy && incoming.price < bestPrice) ||
            (incoming.side == Side::Sell && incoming.price > bestPrice)) {
            break;
        }

        LimitLevel& level = it->second;
        level.fill(incoming, trades);

        if (level.isEmpty()) {
            it = oppositeSide.erase(it);
        } else {
            break; 
        }
    }
}

std::vector<TradeReport> OrderBook::addOrder(Order order) {
    std::vector<TradeReport> trades;

    // 1. Attempt Match
    if (order.side == Side::Buy) {
        match(order, asks_, trades);
    } else {
        match(order, bids_, trades);
    }

    // 2. Add remaining quantity to the book
    if (order.quantity > 0) {
        if (order.side == Side::Buy) {
            // try_emplace avoids default construction. 
            // It looks for order.price; if not found, it creates LimitLevel(order.price)
            auto [it, inserted] = bids_.try_emplace(order.price, order.price);
            it->second.addOrder(order);
        } else {
            auto [it, inserted] = asks_.try_emplace(order.price, order.price);
            it->second.addOrder(order);
        }
    }

    return trades;
}

std::optional<Price> OrderBook::getBestBid() const {
    if (bids_.empty()) return std::nullopt;
    return bids_.begin()->first;
}

std::optional<Price> OrderBook::getBestAsk() const {
    if (asks_.empty()) return std::nullopt;
    return asks_.begin()->first;
}

Volume OrderBook::getVolumeAtPrice(Price price, Side side) const {
    if (side == Side::Buy) {
        auto it = bids_.find(price);
        return (it != bids_.end()) ? it->second.getTotalVolume() : 0;
    } else {
        auto it = asks_.find(price);
        return (it != asks_.end()) ? it->second.getTotalVolume() : 0;
    }
}

} // namespace synthetix