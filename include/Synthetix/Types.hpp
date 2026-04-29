#pragma once
#include <cstdint>

namespace Synthetix {

    enum class Side {
        BUY,
        SELL
    };

    struct Order {
        uint64_t id;
        int32_t price;
        uint32_t quantity;
        Side side;
        bool is_active;

        Order(uint64_t id, int32_t price, uint32_t quantity, Side side)
            : id(id), price(price), quantity(quantity), side(side), is_active(true) {}
    };

} // namespace Synthetix