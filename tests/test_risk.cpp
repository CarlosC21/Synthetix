#include <gtest/gtest.h>
#include "Synthetix/RiskController.hpp"

using namespace synthetix;

class RiskTest : public ::testing::Test {
protected:
    RiskConfig config{
        1000,   // maxOrderSize
        100000, // maxOrderValue
        5000,   // maxNetPosition
        0.05    // 5% maxPriceDeviation
    };
    RiskController risk{config};
};

TEST_F(RiskTest, BlocksLargeOrder) {
    Order giantOrder(1, 100, 5000, Side::Buy); // 5000 > 1000 limit
    EXPECT_EQ(risk.validateOrder(giantOrder, 100.0), RiskStatus::ExceedsOrderSize);
}

TEST_F(RiskTest, BlocksPositionOverload) {
    // Current position: 0. Add 4500 shares.
    risk.updatePosition(Side::Buy, 4500);
    
    // Attempt to buy 1000 more (Total 5500 > 5000 limit)
    Order aggressiveOrder(2, 100, 1000, Side::Buy);
    EXPECT_EQ(risk.validateOrder(aggressiveOrder, 100.0), RiskStatus::ExceedsPositionLimit);
}

TEST_F(RiskTest, AllowsHedgeOrder) {
    // Current position: 4500 (Long)
    risk.updatePosition(Side::Buy, 4500);
    
    // Selling 1000 shares should be SAFE because it reduces our net exposure
    Order hedgeOrder(3, 100, 1000, Side::Sell);
    EXPECT_EQ(risk.validateOrder(hedgeOrder, 100.0), RiskStatus::Safe);
}

TEST_F(RiskTest, BlocksPriceDeviation) {
    // Fair value is 100. Order price 120 is a 20% deviation (Limit is 5%)
    Order badPriceOrder(4, 120, 10, Side::Buy);
    EXPECT_EQ(risk.validateOrder(badPriceOrder, 100.0), RiskStatus::PriceTooFarFromFairValue);
}