#include "Synthetix/Engine.hpp"

#include <iostream>

int main() {
  synthetix::Engine engine;
  synthetix::Order o{1, "ES", 1, 5000};

  engine.on_order(o);

  std::cout << "Synthetix processed_orders=" << engine.processed_orders() << "\n";
  return 0;
}

