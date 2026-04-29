# Synthetix

High-performance trading engine (C++20) with CMake, tests, and benchmarks.

## Build (Windows PowerShell)

Prereqs:
- CMake (3.20+)
- A C++20-capable compiler (Visual Studio 2022 recommended)

Configure + build:

```powershell
cd Synthetix
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build --config Release
```

Run:

```powershell
.\build\Release\synthetix_app.exe
.\build\Release\synthetix_benchmarks.exe
ctest --test-dir build -C Release
```

