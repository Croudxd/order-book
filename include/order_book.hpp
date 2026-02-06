#pragma once
#include <cstddef>
#include <iostream>
#include <vector>
#include <list>
#include <map>

#include "order.hpp"
#include "trade.hpp"


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
            (order.type == 0) ? order.price *= -1 : order.price;
            bool matched = check_match(order);

            if (matched == true)
            {
                execute(std::move(order));
                return;
            }
            auto [ it, inserted ] = lookup[order.type]->try_emplace(order.price);
            it->second.push_back(std::move(order));
            return;
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
                match = order.price*-1 >= it->first ;
            }
            return match;
        }

        void cancel_order () { }

        std::vector<Trade> get_trade_history() { return this->trade_history; }

    private:
        void execute (Order order)
        {
            size_t s = order.size;
            if (lookup[(order.type == 1) ? 0 : 1]->empty() )
            {
                return;
            }
            if (lookup[(order.type == 1) ? 0 : 1]->begin()->second.empty())
            {
                return;
            }
            size_t si = lookup[(order.type == 1) ? 0 : 1]->begin()->second.begin()->size;

            while (lookup[(order.type == 1) ? 0 : 1]->begin()->second.begin()->size != 0 && order.size != 0)
            {
                lookup[(order.type == 1) ? 0 : 1]->begin()->second.begin()->size--;
                order.size--;
            }
            if (lookup[(order.type == 1) ? 0 : 1]->begin()->second.begin()->size == 0 && order.size == 0)
            {

                long price;
                (order.type == 0) ? price  = order.price* -1 : price = order.price;
                lookup[(order.type == 1) ? 0 : 1]->begin()->second.pop_front();
                trade_history.emplace_back(101 /** add time here later*/,s, price, order.type);
                return;
            }
            if (lookup[(order.type == 1) ? 0 : 1]->begin()->second.begin()->size == 0)
            {
                long price;
                (order.type == 0) ? price  = order.price* -1 : price = order.price;
                lookup[(order.type == 1) ? 0 : 1]->begin()->second.pop_front();
                trade_history.emplace_back(101 /** add time here later*/, si, price, lookup[(order.type == 1) ? 0 : 1]->begin()->second.begin()->type);
                add_order(order);
            } if ( order.size == 0)
            {
                trade_history.emplace_back(101 /** add time here later*/, s, lookup[(order.type == 1) ? 0 : 1]->begin()->second.begin()->price, order.type);
                return;
            }
        }

        std::map<long, std::list<Order>> bids;
        std::map<long, std::list<Order>> asks;
        std::map<long, std::list<Order>>* lookup[2];
        std::vector<Trade> trade_history;

};
