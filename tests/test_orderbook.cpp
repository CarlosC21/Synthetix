#include <gtest/gtest.h>
#include "Synthetix/OrderBook.hpp"

using namespace synthetix;

class OrderBookTest : public ::testing::Test {
protected:
    OrderBook book;
};

TEST_F(OrderBookTest, BestBidIsUpdatedOnAdd) {
    Order bid1{ 1001, 150, 10, Side::Buy };
    Order bid2{ 1002, 160, 5,  Side::Buy };

    book.addOrder(bid1);
    book.addOrder(bid2);

    auto bestBid = book.getBestBid();
    ASSERT_TRUE(bestBid.has_value());
    EXPECT_EQ(bestBid.value(), 160); // 160 is higher than 150
}

TEST_F(OrderBookTest, BestAskIsUpdatedOnAdd) {
    Order ask1{ 2001, 170, 10, Side::Sell };
    Order ask2{ 2002, 165, 5,  Side::Sell };

    book.addOrder(ask1);
    book.addOrder(ask2);

    auto bestAsk = book.getBestAsk();
    ASSERT_TRUE(bestAsk.has_value());
    EXPECT_EQ(bestAsk.value(), 165); // 165 is lower (better) than 170
}