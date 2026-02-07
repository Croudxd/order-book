# High-Performance C++ Order Book

A low-latency limit order book and matching engine implemented in C++20.
Designed to handle high-throughput trading scenarios using standard STL containers optimized for cache locality and branchless execution.

## 🚀 Performance Benchmarks (Release)
Tested on a dataset of **100 Million Orders** (Ryzen 9 5000/16GB ram):

| Metric | Performance |
| :--- | :--- |
| **Throughput** | **12,496,097 orders/sec** |
| **Latency** | **80 nanoseconds** (avg) |
| **Execution Time** | 8.00 seconds (100M orders) |
| **Reliability** | 100% Deterministic match rate (81M trades) |

## 🛠️ Technical Implementation
* **Data Structure:** Hybrid approach using `std::map` (Red-Black Tree) for price levels and `std::list` for O(1) insertions/deletions.
* **Optimization:**
    * **Branchless Matching:** Replaced loop-based execution with `std::min` arithmetic for O(1) trade execution.
    * **Negative Key Trick:** Bid/Ask maps unified into a single type using negative key storage, preserving contiguous memory layout without custom comparators.
    * **Zero-Copy:** Extensive use of `std::move` to prevent unnecessary allocations in the hot path.

## 📦 Build & Run

```bash
git clone git@github.com/Croudxd/order-book.git
mkdir build && cd build
cmake -DCMAKE_BUILD_TYPE=Release ..
make -j
./order-book-benchmark
```

## To Do
-  [x] : Implement cancel_order.
-  [ ] : Implement a way to read from a rust program which will get data from bitifinex.
- [ ] : Connect to my rust backtester.
