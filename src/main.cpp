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
#include <vector>



int main ()
{
    Order_book book;

    std::vector<Order> test_orders;

    test_orders.emplace_back(Order_type::sell, 100.0f, 10, 1);
    test_orders.emplace_back(Order_type::sell, 101.0f, 20, 2);
    test_orders.emplace_back(Order_type::buy, 102.0f, 5, 3);
    test_orders.emplace_back(Order_type::buy, 99.0f, 10, 4);

    std::cout << "--- STARTING TRADING ENGINE ---" << std::endl;

    for (auto& ord : test_orders)
    {
        book.add_order(std::move(ord));
    }

    std::cout << "\n--- END OF TRADING DAY ---" << std::endl;

    return 0;
}
