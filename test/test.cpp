#include "order.hpp"
#include "order_book.hpp"
#include <iostream>

int main ()
{
    Order_book book;
    std::vector<Order> test_orders;

    test_orders.emplace_back(Order_type::sell, 100, 10, 1);
    test_orders.emplace_back(Order_type::sell, 102, 20, 2);
    test_orders.emplace_back(Order_type::buy, 98, 10, 3);
    test_orders.emplace_back(Order_type::buy, 97, 10, 3);

    
    for (auto& ord : test_orders) book.add_order(std::move(ord));
    test_orders.clear();

    // Should buy 10 at 100, and 5 at 102
    book.add_order(Order(Order_type::buy, 103, 15, 4));

    // should sell 10 at 98
    book.add_order(Order(Order_type::sell, 95, 15, 5));

    book.add_order(Order(Order_type::sell, 40, 10, 6));

    for (auto& trade : book.get_trade_history())
    {
        std::string side = (trade.type == Order_type::sell) ? "sell" : "buy";
        std::cout << "Trade: " << side << " executed @ " << trade.price << " Size: " << trade.size << std::endl;
    }

    return 0;
}
