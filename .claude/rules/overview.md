# Kosy — Overview

Kosy is a **C++23** and **Python 3.14+** project centered on **math problems** for interview preparation and experimentation. The project is released under the **MIT License**.

## Technical stack

| Layer | Technology |
| ----- | ---------- |
| C++ | C++23, GNU extensions optional, OpenMP where applicable |
| Python | 3.14+, NumPy/SciPy ecosystem, Numba, Cython, PyQt6, Loguru |
| Interop | nanobind |
| Build | CMake 3.31+, Ninja, CMake Presets |
| C++ deps | CPM + vcpkg (doctest, spdlog, cxxopts, Matplot++, nanobind, …) |
| Python deps | uv + `pyproject.toml` / `uv.lock` |
| CI | GitHub Actions |

## Module structure

- **`src/`**: libraries, headers, and C++ module units
- **`tests/`**: C++ tests (`*_test.cpp`) and CMake for tests
- **`cmake/`**: toolchains, CPM third-party definitions, utilities
- **`third_party/`**: CPM cache and vendored sources

## Build system

Configuration is **CMake-first**: use `cmake --preset <name>` then `cmake --build out/build/<name>`. Python workflows use **`uv sync`** and **`uv run pytest`**.
