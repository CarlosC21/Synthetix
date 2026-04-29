#include <gtest/gtest.h>
#include "Synthetix/Types.hpp"
#include "Synthetix/LimitLevel.hpp"

TEST(OrderBookTests, AddOrderIncreasesVolume) {
    Synthetix::LimitLevel level(15000); // Price $1.5000
    Synthetix::Order o1{1, 15000, 100, Synthetix::Side::BUY};
    
    level.addOrder(o1);
    EXPECT_EQ(level.totalVolume(), 100);
}