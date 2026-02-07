#pragma once
#include <cstddef>
#include <iostream>
#include <vector>
#include <list>
#include <map>
#include <unordered_map>

#include "order.hpp"
#include "trade.hpp"


class Order_book
{
    friend class Order;
    public:
        Order_book ()
        {
            lookup[0] = &bids;
            lookup[1] = &asks;
        }

        void add_order (Order order)
        {
            (order.type == 0) ? order.price *= -1 : order.price;
            bool matched = check_match(order);

            if (matched == true)
            {
                if (!lookup[(order.type == 1) ? 0 : 1]->empty() && !lookup[(order.type == 1) ? 0 : 1]->begin()->second.empty())
                {

                    while ( order.size > 0)
                    {
                        if (lookup[(order.type == 1) ? 0 : 1]->empty())
                        {
                            break;
                        }
                        if (!check_match(order))
                        {
                            break;
                        }
                        execute(order);
                    }
                }
            }
            if (order.size > 0)
            {
                auto [ it, inserted ] = lookup[order.type]->try_emplace(order.price);
                it->second.push_back(std::move(order));
                auto list_it = std::prev(it->second.end());

                order_lookup[list_it->ID] = Order_entry{ list_it, (order.type == Order_type::buy)};
                return;
            }

        }

        bool check_match(Order order)
        {
            // Take our order depending on if its bid or ask, we find the lowest (or highest) opposite.
            // Example:
            // if order == bid && order.price >= asks.highest : execute()
            // if order == ask && order.price >= asks.lowest : execute()
            // And then deal with the sizes after .
            int type = (order.type == 1) ? 0 : 1;
            if (lookup[type]->empty()) return false;
            bool match = false;
            auto it = lookup[type]->begin();

            if (order.type == Order_type::sell)
            {
                match = order.price <= it->first*-1;
            }
            if (order.type == Order_type::buy)
            {
                match = order.price*-1 >= it->first ;
            }
            return match;
        }
        //Can now write this.
        void cancel_order (size_t ID)
        {
            auto it = order_lookup[ID];
            // Get the data
            // Get what map its fromo (bid or asks)
            // safely check if a list exists using the data we took out (price)
            // erase the Order object from list using iterator.
            // Check if price list is empty, if it is remove it.
            //
            // erase it from order_lookup 
        }

        std::vector<Trade> get_trade_history() { return this->trade_history; }

    private:
        void execute (Order& order) 
        {
            int type = (order.type == 1) ? 0 : 1;
            Order& book_order = lookup[type]->begin()->second.front();

            long trade_size = (order.size < book_order.size) ? order.size : book_order.size;
            long trade_price = book_order.price;

            order.size -= trade_size;
            book_order.size -= trade_size;

            if (order.type == Order_type::sell)
            {
                trade_price = book_order.price *-1;
            }

            trade_history.emplace_back(101, trade_size, trade_price, order.type);

            if (book_order.size == 0)
            {
                lookup[type]->begin()->second.pop_front();
                if (lookup[type]->begin()->second.empty())
                {
                    lookup[type]->erase(lookup[type]->begin());
                }
            }
        }
        struct Order_entry
        {
            std::list<Order>::iterator location;
            bool is_buy;
        };

        std::map<long, std::list<Order>> bids;
        std::map<long, std::list<Order>> asks;
        std::map<long, std::list<Order>>* lookup[2];
        std::vector<Trade> trade_history;
        std::unordered_map<size_t, Order_entry> order_lookup;

};
