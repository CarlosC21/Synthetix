#include "Synthetix/Engine.hpp"

#include <chrono>
#include <cstdint>
#include <iostream>

int main() {
  constexpr std::uint64_t iterations = 5'000'000;

  synthetix::Engine engine;
  synthetix::Order o{1, "NQ", 1, 123};

  const auto start = std::chrono::steady_clock::now();
  for (std::uint64_t i = 0; i < iterations; ++i) {
    engine.on_order(o);
  }
  const auto end = std::chrono::steady_clock::now();

  const auto ns = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
  const double per_order_ns = static_cast<double>(ns) / static_cast<double>(iterations);

  std::cout << "orders=" << iterations << "\n";
  std::cout << "total_ns=" << ns << "\n";
  std::cout << "ns_per_order=" << per_order_ns << "\n";
  std::cout << "processed_orders=" << engine.processed_orders() << "\n";
  return 0;
}

