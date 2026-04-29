#pragma once
#include <cstdint>
#include <vector>

namespace synthetix {

    // Type aliases for domain clarity
    using Price = int32_t;
    using Volume = uint32_t;
    using OrderID = uint64_t;

    enum class Side : uint8_t {
        Buy,
        Sell
    };

    struct Order {
        OrderID id;
        Price price;
        Volume quantity;
        Side side;
        bool is_active;

        Order() = default;
        Order(OrderID id, Price price, Volume quantity, Side side)
            : id(id), price(price), quantity(quantity), side(side), is_active(true) {}
    };

    // Represents a successful match between two orders
    struct TradeReport {
        OrderID makerId;  // Existing order in the book
        OrderID takerId;  // Incoming order that triggered the match
        Price price;      // Execution price
        Volume quantity;  // Shares/contracts traded
    };

} // namespace synthetix