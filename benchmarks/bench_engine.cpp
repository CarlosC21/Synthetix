#include "Synthetix/Engine.hpp"
#include <chrono>
#include <cstdint>
#include <iostream>
#include <vector>

using namespace synthetix;

int main() {
    // 1. Production-grade Risk Configuration
    // Max order: 1k, Max value: 100k, Max position: 5k, Max deviation: 10%
    RiskConfig config{ 1000, 100000, 5000, 0.10 }; 
    Engine engine(config);

    // 2. Pre-seed the book to ensure the Flat Map has data to traverse
    engine.submitOrder(Order(0, 100, 500, Side::Sell));

    constexpr std::uint64_t iterations = 1'000'000;
    
    // 3. Prepare a valid order (Buy 10 @ 100)
    Order o(1, 100, 10, Side::Buy);

    std::cout << "Starting latency benchmark (" << iterations << " iterations)..." << std::endl;

    const auto start = std::chrono::steady_clock::now();
    
    for (std::uint64_t i = 0; i < iterations; ++i) {
        // We use volatile or a simple check to prevent the compiler from 
        // optimizing away the entire loop since we technically "do nothing" with the result.
        auto [status, trades] = engine.submitOrder(o);
        
        // Update OrderID for each iteration to simulate unique flow
        o.id++; 
    }
    
    const auto end = std::chrono::steady_clock::now();

    // 4. Performance Metrics
    const auto total_ns = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
    const double ns_per_order = static_cast<double>(total_ns) / static_cast<double>(iterations);

    std::cout << "-------------------------------------------" << std::endl;
    std::cout << "Benchmark Results:" << std::endl;
    std::cout << "  Total Iterations : " << iterations << std::endl;
    std::cout << "  Total Time (ms)  : " << total_ns / 1'000'000.0 << std::endl;
    std::cout << "  Latency per Op   : " << ns_per_order << " ns" << std::endl;
    std::cout << "  Final Position   : " << engine.getRisk().getNetPosition() << std::endl;
    std::cout << "-------------------------------------------" << std::endl;

    return 0;
}