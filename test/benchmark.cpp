#include <iostream>
#include <vector>
#include <chrono>
#include <random>
#include "order_book.hpp"
#include "order.hpp"

const int NUM_ORDERS = 100000000; 
const int PRICE_MIN = 95;
const int PRICE_MAX = 105;

int main()
{
    Order_book book;
    std::vector<Order> orders;
    orders.reserve(NUM_ORDERS);

    std::cout << "Generating " << NUM_ORDERS << " random orders..." << std::endl;
    
    std::mt19937 rng(12345); 
    std::uniform_int_distribution<int> type_dist(0, 1);
    std::uniform_int_distribution<int> price_dist(PRICE_MIN, PRICE_MAX);
    std::uniform_int_distribution<int> size_dist(1, 100);

    for (int i = 0; i < NUM_ORDERS; ++i) {
        Order_type type = (type_dist(rng) == 0) ? Order_type::buy : Order_type::sell;
        long price = price_dist(rng);
        size_t size = size_dist(rng);
        orders.emplace_back(type, price, size, i);
    }

    std::cout << "Starting Benchmark..." << std::endl;

    auto start_time = std::chrono::high_resolution_clock::now();

    for (auto& order : orders) {
        book.add_order(std::move(order));
    }

    auto end_time = std::chrono::high_resolution_clock::now();

    auto duration_ns = std::chrono::duration_cast<std::chrono::nanoseconds>(end_time - start_time).count();
    double duration_seconds = duration_ns / 1e9;
    double orders_per_second = NUM_ORDERS / duration_seconds;
    
    std::cout << "\n--------------------------------" << std::endl;
    std::cout << "RESULTS" << std::endl;
    std::cout << "--------------------------------" << std::endl;
    std::cout << "Processed:   " << NUM_ORDERS << " orders" << std::endl;
    std::cout << "Time Taken:  " << duration_seconds << " seconds" << std::endl;
    std::cout << "Throughput:  " << (long)orders_per_second << " orders/sec" << std::endl;
    std::cout << "Latency:     " << (duration_ns / NUM_ORDERS) << " ns/order (avg)" << std::endl;
    std::cout << "--------------------------------" << std::endl;

    std::cout << "Trades Executed: " << book.get_trade_history().size() << std::endl;

    return 0;
}
