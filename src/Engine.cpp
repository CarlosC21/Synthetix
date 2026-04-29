#include "Synthetix/Engine.hpp"

namespace synthetix {

void Engine::on_order(const Order& /*order*/) noexcept {
  ++processed_orders_;
}

}  // namespace synthetix

