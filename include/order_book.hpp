#pragma once
#include <cstddef>
#include <iostream>
#include <list>
#include <map>
#include "order.hpp"


class Order_book
{
    friend class Order;
    public:
        Order_book ()
        {
            lookup[0] = &asks;
            lookup[1] = &bids;
        }

        void add_order (Order order)
        {
            bool matched = check_match(order);

            if (matched == true)
            {
                execute();
                return;
            }

            auto [ it, inserted ] = lookup[order.type]->try_emplace(order.price);
            it->second.push_back(std::move(order));
        }

        bool check_match(Order order)
        {
            // Take our order depending on if its bid or ask, we find the lowest (or highest) opposite.
            // Example:
            // if order == bid && order.price >= asks.highest : execute()
            // if order == ask && order.price >= asks.lowest : execute()
            // And then deal with the sizes after .
            bool match = false;
            auto it = lookup[(order.type == 1) ? 0 : 1]->begin();

            if (order.type == Order_type::sell)
            {
                match = order.price == it->first ;
            }
            if (order.type == Order_type::buy)
            {
                match = order.price >= it->first ;
            }

            return match;
        }

        void cancel_order () { }

    private:
        void execute ( )
        {
            //reduce quantity
        }

        std::map<size_t, std::list<Order>> bids;
        std::map<size_t, std::list<Order>> asks;
        std::map<size_t, std::list<Order>>* lookup[2];

};
