#include "order.hpp"
#include "order_book.hpp"
#include <iostream>

int main ()
{
    Order_book book;
    std::vector<Order> test_orders;

    std::cout << "--- 1. BUILDING LIQUIDITY ---" << std::endl;
    // Seller at 100 (Cheapest)
    test_orders.emplace_back(Order_type::sell, 100, 10, 1);
    // Seller at 102 (Expensive)
    test_orders.emplace_back(Order_type::sell, 102, 20, 2);
    // Buyer at 98 (Highest Bid)
    test_orders.emplace_back(Order_type::buy, 98, 50, 3);
    
    for (auto& ord : test_orders) book.add_order(std::move(ord));
    test_orders.clear();

    std::cout << "--- 2. EXECUTING TRADES ---" << std::endl;

    // SCENARIO A: Aggressive Buyer
    // Buys 15 shares at limit 103.
    // Should eat the 10 shares at 100, then 5 shares at 102.
    std::cout << ">> Incoming BUY 15 @ 103" << std::endl;
    book.add_order(Order(Order_type::buy, 103, 15, 4));

    // SCENARIO B: Aggressive Seller (Price Improvement)
    // Sells 10 shares at limit 95.
    // Should match the Buyer at 98 (Better price).
    std::cout << ">> Incoming SELL 10 @ 95" << std::endl;
    book.add_order(Order(Order_type::sell, 95, 10, 5));

    std::cout << "\n--- TRADE LOG ---" << std::endl;
    for (auto& trade : book.get_trade_history())
    {
        std::string side = (trade.type == Order_type::sell) ? "sell" : "buy";
        std::cout << "Trade: " << side << " executed @ " << trade.price << " Size: " << trade.size << std::endl;
    }

    return 0;
}
