#pragma once

#include <cstdint>
#include <string_view>

namespace synthetix {

struct Order {
  std::uint64_t id{};
  std::string_view symbol{};
  std::int64_t qty{};
  std::int64_t price_ticks{};
};

class Engine final {
public:
  Engine() = default;

  [[nodiscard]] std::uint64_t processed_orders() const noexcept { return processed_orders_; }
  void on_order(const Order& order) noexcept;

private:
  std::uint64_t processed_orders_{0};
};

}  // namespace synthetix

