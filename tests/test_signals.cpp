#include <gtest/gtest.h>
#include "Synthetix/Signals.hpp"

using namespace synthetix;

class SignalTest : public ::testing::Test {
protected:
    SignalEngine engine;
};

// Test Micro-price logic: Neutral market (Equal volume)
TEST_F(SignalTest, MicroPriceNeutral) {
    // Bid: 100 @ 10, Ask: 100 @ 12
    // Mid-price: 11, Micro-price: (10*100 + 12*100) / 200 = 11
    engine.updateSignals(10, 100, 12, 100);
    
    auto mp = engine.getMicroPrice();
    ASSERT_TRUE(mp.has_value());
    EXPECT_DOUBLE_EQ(mp.value(), 11.0);
}

// Test Micro-price logic: Skewed market (Heavy Bid)
TEST_F(SignalTest, MicroPriceSkewedBid) {
    // Bid: 900 @ 10, Ask: 100 @ 11
    // High bid volume should push micro-price closer to the Ask
    // (10*100 + 11*900) / 1000 = 10.9
    engine.updateSignals(10, 900, 11, 100);
    
    auto mp = engine.getMicroPrice();
    EXPECT_DOUBLE_EQ(mp.value(), 10.9);
}

// Test OFI: Positive Imbalance (Buy pressure)
TEST_F(SignalTest, OFIBuyPressure) {
    // Initial State: 100 @ 10 | 100 @ 11
    engine.updateSignals(10, 100, 11, 100);
    
    // Update: Bid volume increases to 200
    // DeltaBid = 200 - 100 = 100. DeltaAsk = 100 - 100 = 0.
    // OFI = 100 - 0 = 100
    engine.updateSignals(10, 200, 11, 100);
    EXPECT_DOUBLE_EQ(engine.getOFI(), 100.0);
}

// Test OFI: Price Jump (Aggressive buying)
TEST_F(SignalTest, OFIPriceRise) {
    // Initial State: 100 @ 10 | 100 @ 11
    engine.updateSignals(10, 100, 11, 100);
    
    // Update: Bid price jumps to 11 (taking out the old ask)
    // Bid @ 11 with 50 volume. 
    // Since bP > prev_bP, deltaBid = current_vol = 50.
    // Since aP > prev_aP (let's say ask moved to 12), deltaAsk = -prev_vol = -100.
    // OFI = 50 - (-100) = 150
    engine.updateSignals(11, 50, 12, 100);
    EXPECT_DOUBLE_EQ(engine.getOFI(), 150.0);
}

// Test handling of empty book
TEST_F(SignalTest, EmptyBookHandlesNull) {
    engine.updateSignals(0, 0, 0, 0);
    EXPECT_FALSE(engine.getMicroPrice().has_value());
    EXPECT_DOUBLE_EQ(engine.getSpread(), 0.0);
}