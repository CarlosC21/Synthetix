cmake_minimum_required(VERSION 3.20)

project(Synthetix
  VERSION 0.1.0
  DESCRIPTION "High-performance trading engine"
  LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
cmake_policy(SET CMP0135 NEW)

option(SYNT_ENABLE_TESTS "Build unit tests" ON)
option(SYNT_ENABLE_BENCHMARKS "Build benchmarks" ON)

# ---------------------------------------------------------------------------
# Core Library: The engine logic
# ---------------------------------------------------------------------------
add_library(synthetix
  src/Engine.cpp
  src/OrderBook.cpp  # Added for Phase 2
  src/Signals.cpp
  src/RiskController.cpp
)
add_library(Synthetix::synthetix ALIAS synthetix)

target_include_directories(synthetix
  PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

target_compile_features(synthetix PUBLIC cxx_std_20)

if (MSVC)
  target_compile_options(synthetix PRIVATE /W4 /permissive-)
else()
  target_compile_options(synthetix PRIVATE -Wall -Wextra -Wpedantic)
endif()

# ---------------------------------------------------------------------------
# Main Application
# ---------------------------------------------------------------------------
add_executable(synthetix_app
  src/main.cpp
)
target_link_libraries(synthetix_app PRIVATE Synthetix::synthetix)

# ---------------------------------------------------------------------------
# Testing Suite
# ---------------------------------------------------------------------------
include(CTest)

if (SYNT_ENABLE_TESTS AND BUILD_TESTING)
  include(FetchContent)

  # Prevent GoogleTest from overriding parent settings on Windows
  set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

  FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
  )
  FetchContent_MakeAvailable(googletest)

  enable_testing()

  add_executable(synthetix_tests
    tests/test_smoke.cpp
    tests/test_orderbook.cpp
    tests/test_matching.cpp
    tests/test_signals.cpp
    tests/test_risk.cpp
  )
  target_link_libraries(synthetix_tests PRIVATE Synthetix::synthetix GTest::gtest_main)

  include(GoogleTest)
  gtest_discover_tests(synthetix_tests)
endif()

# ---------------------------------------------------------------------------
# Benchmarking Suite
# ---------------------------------------------------------------------------
if (SYNT_ENABLE_BENCHMARKS)
  add_executable(synthetix_benchmarks
    benchmarks/bench_engine.cpp
  )
  target_link_libraries(synthetix_benchmarks PRIVATE Synthetix::synthetix)
endif()