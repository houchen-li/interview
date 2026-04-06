<h1 align="center">Kosy</h1>

<p align="center">
  <b>A Collection of Interesting Math Problems for Interview Preparation.</b>
</p>

<p align="center">
  <a href="README_zh.md">中文版</a> •
  <a href="LICENSE">MIT License</a> •
  <a href="https://github.com/houchen-li/kosy">GitHub</a>
</p>

<p align="center">
  <img src="https://img.shields.io/badge/C%2B%2B-23-blue.svg" alt="C++ Standard">
  <img src="https://img.shields.io/badge/Python-≥3.14-3776AB.svg?logo=python&logoColor=white" alt="Python">
  <img src="https://img.shields.io/badge/CMake-≥3.31-064F8C.svg?logo=cmake&logoColor=white" alt="CMake">
  <img src="https://img.shields.io/badge/license-MIT-yellow.svg" alt="License">
  <img src="https://img.shields.io/badge/Platform-Linux%20|%20macOS%20|%20Windows-lightgrey.svg" alt="Platform">
</p>

---

## Introduction

Kosy is a modern **C++23 / Python** hybrid project featuring interesting math problems — primarily for interview preparation, algorithmic exploration, and educational purposes. Each problem is implemented with a focus on clarity, performance, and modern language features.

**C++ implementations** leverage modern templates, concepts, and compile-time evaluation to showcase how complex mathematical algorithms can be expressed cleanly with C++23. **Python implementations** use scientific computing libraries for rapid prototyping and rich visualization.

Key capabilities:

- **Monte Carlo methods** — volume estimation for arbitrary geometric solids using random sampling and importance analysis.
- **Complex analysis & fractals** — Newton-Raphson iteration on the complex plane, producing Newton fractal visualizations.
- **C++/Python interop** — seamless bridge between high-performance C++ and rapid-prototyping Python via [nanobind](https://github.com/wjakob/nanobind).

## Project Structure

```
kosy/
├── src/                          # Source files
│   ├── calculate_volume.cpp      # C++ — Monte Carlo volume estimation
│   └── julia_set.py              # Python — Newton fractal visualization
├── tests/                        # Test suite (C++ doctest / Python pytest)
├── cmake/
│   ├── toolchains/               # Cross-platform toolchain files
│   │   ├── linux-gcc-x64.cmake
│   │   ├── linux-clang-x64.cmake
│   │   ├── darwin-gcc-arm64.cmake
│   │   ├── darwin-clang-arm64.cmake
│   │   └── windows-msvc-x64.cmake
│   ├── third_party/              # CPM dependency declarations
│   └── utils.cmake               # CMake helper functions
├── CMakeLists.txt                # C++ build configuration
├── CMakePresets.json             # CMake build presets
├── pyproject.toml                # Python project configuration (uv)
├── uv.lock                       # Locked Python dependencies
├── .clang-format                 # C++ formatting rules (Microsoft style)
├── .clang-tidy                   # C++ static analysis configuration
└── .clangd                       # Language server configuration
```

## Prerequisites

The following tools must be installed on your system:

| Tool | Purpose |
|------|---------|
| **CMake** (≥ 3.31) | Build system generator |
| **Ninja** | Build backend (used by all presets) |
| **GCC** (≥ 14) or **Clang** (≥ 22) | C++23-capable compiler |
| **Python** (≥ 3.14) | Python runtime |
| **[uv](https://docs.astral.sh/uv/)** | Python package manager |
| **clangd** | Language server for IDE integration |
| **clang-tidy** | Static analysis |
| **clang-format** | Code formatting |

All C++ dependencies are automatically managed by [CPM.cmake](https://github.com/cpm-cmake/CPM.cmake) and do not require manual installation. Python dependencies are managed by uv via `pyproject.toml`.

## Build

### C++ (CMake)

```bash
# Configure (Linux + GCC)
cmake --preset linux-gcc-x64

# Build
cmake --build --preset linux-gcc-x64

# Run tests
ctest --preset linux-gcc-x64
```

For Clang:

```bash
cmake --preset linux-clang-x64
cmake --build --preset linux-clang-x64
ctest --preset linux-clang-x64
```

### Python (uv)

```bash
# Set up environment and install dependencies
uv sync

# Run tests
uv run pytest

# Run a specific script
uv run python src/julia_set.py
```

### CMake Options

| Option | Default | Description |
|--------|---------|-------------|
| `KOSY_BUILD_TESTING` | `OFF` | Build unit tests |
| `KOSY_ENABLE_INSTALL` | `ON` | Enable install targets |
| `BUILD_SHARED_LIBS` | `OFF` | Build shared libraries |
| `CMAKE_UNITY_BUILD` | `OFF` | Enable unity build for faster compilation |

### Available Presets

| Preset | Platform | Compiler |
|--------|----------|----------|
| `linux-gcc-x64` | Linux x86_64 | GCC |
| `linux-clang-x64` | Linux x86_64 | Clang |
| `darwin-gcc-arm64` | macOS ARM64 | GCC |
| `darwin-clang-arm64` | macOS ARM64 | Clang |
| `windows-msvc-x64` | Windows x86_64 | MSVC |

### CMake Helper Functions

Defined in `cmake/utils.cmake`:

```cmake
kosy_cxx_library(
    NAME library_name
    HDRS header.hpp
    SRCS source.cpp
    DEPS dependency
    PUBLIC
)

kosy_cxx_test(
    NAME component_test
    SRCS component_test.cpp
    DEPS component_library
)
```

## Examples

### Monte Carlo Volume Estimation (C++)

Estimates the volume of geometric solids using Monte Carlo random sampling with modern C++23 templates and concepts:

```cpp
#include <array>
#include <concepts>
#include <format>
#include <iostream>
#include <random>

// 3D random point scatter with configurable bounds and RNG
template <std::floating_point T>
class [[nodiscard]] Scatter final {
  public:
    using value_type = T;
    explicit Scatter(const std::array<value_type, 6>& bounds) noexcept;
    auto operator()() noexcept -> Vec3<value_type>;
    // ...
};

// Ellipsoid: x²/a² + y²/b² + z²/c² ≤ 1
template <std::floating_point T>
struct Ellipsoid final {
    value_type a, b, c;
    auto volume() const noexcept -> value_type {
        // Monte Carlo: sample N points in bounding box,
        // count how many fall inside the ellipsoid
        // Volume ≈ (count / N) × bounding_box_volume
    }
};

Ellipsoid<double> ellipsoid{.a = 3.0, .b = 2.0, .c = 1.0};
double vol = ellipsoid.volume();  // ≈ 4π/3 × 3 × 2 × 1 ≈ 25.13
```

### Newton Fractal Visualization (Python)

Visualizes convergence basins of Newton-Raphson iteration on the complex plane for the polynomial f(z) = z³ - 1:

```python
from itertools import product
import matplotlib.pyplot as plt
import numpy as np

class Polynomial:
    """Polynomial with evaluation, derivative, and Newton-Raphson solver."""

    def __init__(self, coeffs):
        self.coeffs = coeffs

    def eval(self, z):
        result = self.coeffs[0]
        power = 1.0
        for c in self.coeffs[1:]:
            power *= z
            result += c * power
        return result

    def newton_raphson(self, z0, *, abs_tol=1e-12, max_iter=50):
        z = z0
        for i in range(max_iter):
            dz = -self.eval(z) / self.derivative(z)
            z += dz
            if abs(dz) ** 2 < abs_tol ** 2:
                break
        return z, dz, i

# f(z) = z³ - 1 has roots at 1, e^(2πi/3), e^(4πi/3)
f = Polynomial([-1.0, 0.0, 0.0, 1.0])

# Iterate over complex grid and plot convergence iterations
z0 = np.empty((1001, 1001), dtype=np.complex128)
z0.real, z0.imag = np.meshgrid(
    np.linspace(-2, 2, 1001), np.linspace(-2, 2, 1001)
)
# ... produces a beautiful fractal pattern showing convergence basins
```

## C++ Dependencies

| Library | Version | Purpose | Homepage |
|---------|---------|---------|----------|
| [doctest](https://github.com/doctest/doctest) | 2.4.12 | Fast C++ testing framework | https://github.com/doctest/doctest |
| [spdlog](https://github.com/gabime/spdlog) | 1.17.0 | Fast C++ logging library | https://github.com/gabime/spdlog |
| [cxxopts](https://github.com/jarro2783/cxxopts) | 3.3.1 | Command-line option parsing | https://github.com/jarro2783/cxxopts |
| [Matplot++](https://github.com/alandefreitas/matplotplusplus) | 1.2.2 | C++ data visualization | https://github.com/alandefreitas/matplotplusplus |
| [nanobind](https://github.com/wjakob/nanobind) | 2.6.1 | C++/Python interop (minimal overhead) | https://github.com/wjakob/nanobind |
| OpenMP | (system) | Shared-memory parallelism | https://www.openmp.org/ |

## Python Dependencies

| Package | Purpose | Homepage |
|---------|---------|----------|
| [NumPy](https://numpy.org/) | N-dimensional arrays, numerical computing | https://numpy.org/ |
| [SciPy](https://scipy.org/) | Scientific algorithms (optimization, interpolation) | https://scipy.org/ |
| [Matplotlib](https://matplotlib.org/) | Plotting and data visualization | https://matplotlib.org/ |
| [Numba](https://numba.pydata.org/) | JIT compilation for performance acceleration | https://numba.pydata.org/ |
| [Cython](https://cython.org/) | C extensions for Python | https://cython.org/ |
| [PyQt6](https://www.riverbankcomputing.com/software/pyqt/) | GUI support | https://www.riverbankcomputing.com/software/pyqt/ |
| [pytest](https://docs.pytest.org/) | Testing framework | https://docs.pytest.org/ |
| [Loguru](https://github.com/Delgan/loguru) | Simplified logging | https://github.com/Delgan/loguru |

## Development Tools

| Tool | Version | Purpose |
|------|---------|---------|
| [clang-format](https://clang.llvm.org/docs/ClangFormat.html) | 22+ | C++ code formatting (Microsoft style, 100 columns) |
| [clang-tidy](https://clang.llvm.org/extra/clang-tidy/) | 22+ | C++ static analysis |
| [ruff](https://docs.astral.sh/ruff/) | latest | Python linting and formatting |
| [pyright](https://github.com/microsoft/pyright) | latest | Python type checking |

## License

This project is licensed under the [MIT License](LICENSE).

## Acknowledgments

Kosy is built upon the following outstanding open-source projects. We extend our sincere gratitude to their developers and communities:

| Project | Description | Homepage |
|---------|-------------|----------|
| [CPM.cmake](https://github.com/cpm-cmake/CPM.cmake) | CMake package manager | https://github.com/cpm-cmake/CPM.cmake |
| [doctest](https://github.com/doctest/doctest) | Fast C++ testing framework | https://github.com/doctest/doctest |
| [spdlog](https://github.com/gabime/spdlog) | Fast C++ logging library | https://github.com/gabime/spdlog |
| [cxxopts](https://github.com/jarro2783/cxxopts) | Command-line option parsing | https://github.com/jarro2783/cxxopts |
| [Matplot++](https://github.com/alandefreitas/matplotplusplus) | C++ data visualization library | https://github.com/alandefreitas/matplotplusplus |
| [nanobind](https://github.com/wjakob/nanobind) | Minimal C++/Python interop | https://github.com/wjakob/nanobind |
| [NumPy](https://numpy.org/) | Fundamental package for scientific computing | https://numpy.org/ |
| [Matplotlib](https://matplotlib.org/) | Comprehensive plotting library | https://matplotlib.org/ |
