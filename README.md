# Synthetix | High-Performance C++20 Trading Engine

Synthetix is a low-latency, event-driven trading engine engineered for sub-microsecond execution. Designed with a focus on Data-Oriented Design (DOD) and Mechanical Sympathy, the system minimizes cache misses and pointer chasing by utilizing contiguous memory structures. This project demonstrates mastery over market microstructure, real-time risk management, and hardware-optimized C++ architecture.

# Performance Highlights

* The "Map Tax" Solution: Replaced traditional node-based std::map with Flat Maps (sorted std::vector), ensuring the Best Bid/Offer (BBO) and the top of the book reside in the L1 cache for $O(1)$ access.

* Zero-Allocation Path: The core execution loop is designed to minimize heap churn during active trading sessions.

* Low-Latency Orchestration: A final engine class allows for compiler devirtualization, reducing branch mispredictions in the hot path

# Core Architecture

1. High-Performance Matching Engine
A FIFO (Price-Time Priority) engine built for speed.
* Contiguous Memory: Uses a binary-searched flat-vector architecture to maintain order priority while maximizing cache locality.
* Full Execution Cycle: Supports aggressive orders, passive resting orders, partial fills, and liquidity sweeps across multiple levels.

2. Quantitative Signal Engine
Synthetix doesn't just execute; it interprets the market's hidden intent.
* Micro-price: A volume-weighted fair value that accounts for book pressure:
$$P_{\mu} = \frac{P_b \cdot V_a + P_a \cdot V_b}{V_b + V_a}$$
* Order Flow Imbalance (OFI): Real-time tracking of net pressure at the BBO to identify exhaustion and momentum before price moves occur.

3. Institutional-Grade Risk Controller
A mandatory pre-trade gatekeeper that protects the firm from catastrophic failure.
* Fat-Finger Protection: Thresholds for maximum order size and value.
* Price Deviation: Rejects orders that deviate significantly (e.g., >5%) from the current Micro-price.
* Position Accounting: Real-time monitoring of net exposure to enforce hard position limits.

# Technical Stack
* Language: C++20
* Build System: CMake 3.20+
* Compiler: MSVC 2026 / Clang 16+
* Testing: GoogleTest (GTest) for 100% core logic coverage.
* Methodology: Data-Oriented Design (DOD) & Test-Driven Development (TDD). 

# Repository Structure
Synthetix/
├── benchmarks/      # Latency measurement scripts
├── include/         # Header-only interfaces (Signals, Risk, OrderBook)
├── src/             # Implementation of the core orchestration logic
├── tests/           # 16+ Unit tests for Risk, Matching, and Signals
└── CMakeLists.txt   # Build configuration

# Building & Testing
Prerequisites
* CMake 3.20 or higher
* A C++20 compliant compiler (MSVC, GCC, or Clang)

Build the Project
cmake -B build
cmake --build build --config Release

Run Unit Tests
./build/Debug/synthetix_tests.exe

Run Performance Benchmarks
./build/Release/bench_engine.exe

Demo Output
Synthetix Engine Initialized...
Initial Fair Value (Micro-price): 102.35

Attempting Order 3 (Price: 130)...
>>> RISK ALERT: Order 3 rejected due to Price Deviation.

Attempting Order 4 (Aggressive Buy at 105)...
>>> EXECUTION SUCCESS: Filled 200 shares.
New Position: 200 shares.