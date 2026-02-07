/** 
 * Okay we have two parts to an order book.
 * The bids which is sorted from highest to lowest (highest at the top)
 * and the asks which is sorted lowest to highest.
 *
 * The gap between bid and asks is called the spread.
 *
 * At each price point we might have multiple orders, so we store them in time order (FIFO)
 * When we get an incoming bid. we try and match. If it matches we call execute.
 * Else we add the bid into the queue.
 *
 * A user might want to cancel an order, so we need to implement a way to find and cancel it.
 *
 * we might have a market order, basically buy x amount of shares without caring for price.
 * So we pick the top 
 *
 * an order needs. An ID, price, size and a side (buying or selling)
 * */

#include "order.hpp"
#include "order_book.hpp"
#include <cstddef>
#include <vector>
#include <cstdlib> 
#include <ctime> 


int main ()
{
    Order_book book;

    std::vector<Order> test_orders;

    std::srand(std::time(nullptr)); 

    for (int i = 0; i < 100; ++i) 
    {
        Order_type type = (std::rand() % 2 == 0) ? Order_type::buy : Order_type::sell;
        size_t price = 95 + (std::rand() % 11); 
        size_t quantity = 1 + (std::rand() % 50);
        size_t id = i + 1;
        test_orders.emplace_back(type, price, quantity, id);
    }


    for (auto& ord : test_orders)
    {
        book.add_order(std::move(ord));
    }


    for (auto& trade : book.get_trade_history())
    {
        std::string type;
        ( trade.type > 0) ? type="sell" : type="buy";
        std::cout << "Type: "<< type << " Price: "<< trade.price << " Size:"<< trade.size << std::endl;
    }

    return 0;
}
