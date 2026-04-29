#include <gtest/gtest.h>
#include "Synthetix/LimitLevel.hpp"
#include "Synthetix/Types.hpp"

using namespace synthetix;

TEST(SmokeTest, AddOrderIncreasesVolume) {
    LimitLevel level(100);
    
    // Updated to use Side::Buy and our new default constructor/types
    Order o1(1, 100, 50, Side::Buy);
    level.addOrder(o1);

    // Updated to use the new method name getTotalVolume()
    EXPECT_EQ(level.getTotalVolume(), 50);
}

TEST(SmokeTest, MultipleOrdersIncreaseVolume) {
    LimitLevel level(150);
    
    Order o1(1, 150, 100, Side::Sell);
    Order o2(2, 150, 200, Side::Sell);
    
    level.addOrder(o1);
    level.addOrder(o2);

    EXPECT_EQ(level.getTotalVolume(), 300);
}