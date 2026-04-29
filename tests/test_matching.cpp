#include <gtest/gtest.h>
#include "Synthetix/OrderBook.hpp"

using namespace synthetix;

class MatchingTest : public ::testing::Test {
protected:
    OrderBook book;
};

// Test 1: Partial Fill Logic
TEST_F(MatchingTest, PartialFillLeavesResidualVolume) {
    // Passive: 100 shares for sale at 150
    book.addOrder(Order{1, 150, 100, Side::Sell});

    // Aggressive: Want to buy 40 shares at 150
    auto trades = book.addOrder(Order{2, 150, 40, Side::Buy});

    // Verify Trade Report
    ASSERT_EQ(trades.size(), 1);
    EXPECT_EQ(trades[0].quantity, 40);
    EXPECT_EQ(trades[0].price, 150);
    EXPECT_EQ(trades[0].makerId, 1);

    // Verify Book State
    EXPECT_EQ(book.getVolumeAtPrice(150, Side::Sell), 60); // 100 - 40 = 60
    EXPECT_EQ(book.getBestAsk(), 150);
}

// Test 2: Liquidity Sweep (Multi-level Match)
TEST_F(MatchingTest, LargeOrderSweepsMultipleLevels) {
    // Set up the "Sell Side" wall
    book.addOrder(Order{1, 101, 50, Side::Sell}); // 50 @ 101
    book.addOrder(Order{2, 102, 50, Side::Sell}); // 50 @ 102
    book.addOrder(Order{3, 103, 50, Side::Sell}); // 50 @ 103

    // Aggressive: Buy 120 shares at 105 (willing to pay up to 105)
    auto trades = book.addOrder(Order{4, 105, 120, Side::Buy});

    // We should have 3 trade reports (fully consumed 101 and 102, partially consumed 103)
    ASSERT_EQ(trades.size(), 3);
    
    EXPECT_EQ(trades[0].price, 101);
    EXPECT_EQ(trades[0].quantity, 50);

    EXPECT_EQ(trades[1].price, 102);
    EXPECT_EQ(trades[1].quantity, 50);

    EXPECT_EQ(trades[2].price, 103);
    EXPECT_EQ(trades[2].quantity, 20); // Remainder of the 120

    // Verify Book State
    EXPECT_EQ(book.getBestAsk(), 103);
    EXPECT_EQ(book.getVolumeAtPrice(103, Side::Sell), 30); // 50 - 20 = 30
    EXPECT_EQ(book.getVolumeAtPrice(101, Side::Sell), 0);
}

// Test 3: Price-Time Priority (FIFO)
TEST_F(MatchingTest, FirstOrderAtPriceIsFilledFirst) {
    book.addOrder(Order{1, 100, 50, Side::Sell}); // Person A
    book.addOrder(Order{2, 100, 50, Side::Sell}); // Person B

    // Buy 60 shares
    auto trades = book.addOrder(Order{3, 100, 60, Side::Buy});

    ASSERT_EQ(trades.size(), 2);
    EXPECT_EQ(trades[0].makerId, 1); // Person A filled completely
    EXPECT_EQ(trades[1].makerId, 2); // Person B filled partially (10 shares)
    EXPECT_EQ(trades[1].quantity, 10);
}