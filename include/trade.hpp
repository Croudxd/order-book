#pragma once
#include "order.hpp"
#include <cstddef>

struct Trade 
{
    size_t time;
    size_t size;
    long price;
    Order_type type;
};


