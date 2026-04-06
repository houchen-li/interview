<h1 align="center">Kosy</h1>

<p align="center">
  <b>一组有趣的数学问题合集，面向面试准备。</b>
</p>

<p align="center">
  <a href="README.md">English</a> •
  <a href="LICENSE">MIT 许可证</a> •
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

## 简介

Kosy 是一个现代 **C++23 / Python** 混合编程项目，汇集了有趣的数学问题 — 主要面向面试准备、算法探索和教学用途。每道题目的实现都注重代码清晰度、运行性能和现代语言特性的运用。

**C++ 实现**充分利用现代模板、Concepts 和编译期求值，展示如何用 C++23 简洁地表达复杂的数学算法。**Python 实现**借助科学计算库进行快速原型开发和丰富的可视化。

主要能力：

- **蒙特卡洛方法** — 利用随机采样估算任意几何体的体积。
- **复分析与分形** — 在复平面上进行 Newton-Raphson 迭代，生成 Newton 分形可视化图。
- **C++/Python 互操作** — 通过 [nanobind](https://github.com/wjakob/nanobind) 实现高性能 C++ 与快速原型 Python 之间的无缝桥接。

## 项目结构

```
kosy/
├── src/                          # 源代码
│   ├── calculate_volume.cpp      # C++ — 蒙特卡洛体积估算
│   └── julia_set.py              # Python — Newton 分形可视化
├── tests/                        # 测试（C++ doctest / Python pytest）
├── cmake/
│   ├── toolchains/               # 跨平台工具链文件
│   │   ├── linux-gcc-x64.cmake
│   │   ├── linux-clang-x64.cmake
│   │   ├── darwin-gcc-arm64.cmake
│   │   ├── darwin-clang-arm64.cmake
│   │   └── windows-msvc-x64.cmake
│   ├── third_party/              # CPM 依赖声明
│   └── utils.cmake               # CMake 辅助函数
├── CMakeLists.txt                # C++ 构建配置
├── CMakePresets.json             # CMake 构建预设
├── pyproject.toml                # Python 项目配置（uv）
├── uv.lock                       # Python 依赖锁定文件
├── .clang-format                 # C++ 格式化规则（Microsoft 风格）
├── .clang-tidy                   # C++ 静态分析配置
└── .clangd                       # 语言服务器配置
```

## 前置依赖

以下工具需要预先安装在系统中：

| 工具 | 用途 |
|------|------|
| **CMake**（≥ 3.31） | 构建系统生成器 |
| **Ninja** | 构建后端（所有预设均使用） |
| **GCC**（≥ 14）或 **Clang**（≥ 22） | 支持 C++23 的编译器 |
| **Python**（≥ 3.14） | Python 运行时 |
| **[uv](https://docs.astral.sh/uv/)** | Python 包管理器 |
| **clangd** | 语言服务器，用于 IDE 集成 |
| **clang-tidy** | 静态分析 |
| **clang-format** | 代码格式化 |

所有 C++ 依赖均由 [CPM.cmake](https://github.com/cpm-cmake/CPM.cmake) 自动管理，无需手动安装。Python 依赖通过 uv 和 `pyproject.toml` 管理。

## 构建

### C++（CMake）

```bash
# 配置（Linux + GCC）
cmake --preset linux-gcc-x64

# 编译
cmake --build --preset linux-gcc-x64

# 运行测试
ctest --preset linux-gcc-x64
```

使用 Clang：

```bash
cmake --preset linux-clang-x64
cmake --build --preset linux-clang-x64
ctest --preset linux-clang-x64
```

### Python（uv）

```bash
# 配置环境并安装依赖
uv sync

# 运行测试
uv run pytest

# 运行指定脚本
uv run python src/julia_set.py
```

### CMake 选项

| 选项 | 默认值 | 说明 |
|------|--------|------|
| `KOSY_BUILD_TESTING` | `OFF` | 构建单元测试 |
| `KOSY_ENABLE_INSTALL` | `ON` | 启用安装目标 |
| `BUILD_SHARED_LIBS` | `OFF` | 构建动态库 |
| `CMAKE_UNITY_BUILD` | `OFF` | 启用 Unity Build 加速编译 |

### 可用预设

| 预设 | 平台 | 编译器 |
|------|------|--------|
| `linux-gcc-x64` | Linux x86_64 | GCC |
| `linux-clang-x64` | Linux x86_64 | Clang |
| `darwin-gcc-arm64` | macOS ARM64 | GCC |
| `darwin-clang-arm64` | macOS ARM64 | Clang |
| `windows-msvc-x64` | Windows x86_64 | MSVC |

### CMake 辅助函数

定义在 `cmake/utils.cmake` 中：

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

## 示例

### 蒙特卡洛体积估算（C++）

使用现代 C++23 模板和 Concepts，通过蒙特卡洛随机采样估算几何体的体积：

```cpp
#include <array>
#include <concepts>
#include <format>
#include <iostream>
#include <random>

// 可配置边界和随机数生成器的 3D 随机点散布器
template <std::floating_point T>
class [[nodiscard]] Scatter final {
  public:
    using value_type = T;
    explicit Scatter(const std::array<value_type, 6>& bounds) noexcept;
    auto operator()() noexcept -> Vec3<value_type>;
    // ...
};

// 椭球体：x²/a² + y²/b² + z²/c² ≤ 1
template <std::floating_point T>
struct Ellipsoid final {
    value_type a, b, c;
    auto volume() const noexcept -> value_type {
        // 蒙特卡洛：在包围盒中采样 N 个点，
        // 统计落入椭球体内部的数量
        // 体积 ≈ (count / N) × 包围盒体积
    }
};

Ellipsoid<double> ellipsoid{.a = 3.0, .b = 2.0, .c = 1.0};
double vol = ellipsoid.volume();  // ≈ 4π/3 × 3 × 2 × 1 ≈ 25.13
```

### Newton 分形可视化（Python）

在复平面上对多项式 f(z) = z³ - 1 进行 Newton-Raphson 迭代，可视化收敛盆地：

```python
from itertools import product
import matplotlib.pyplot as plt
import numpy as np

class Polynomial:
    """支持求值、求导和 Newton-Raphson 求解的多项式类。"""

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

# f(z) = z³ - 1 的根为 1, e^(2πi/3), e^(4πi/3)
f = Polynomial([-1.0, 0.0, 0.0, 1.0])

# 在复数网格上迭代并绘制收敛次数
z0 = np.empty((1001, 1001), dtype=np.complex128)
z0.real, z0.imag = np.meshgrid(
    np.linspace(-2, 2, 1001), np.linspace(-2, 2, 1001)
)
# ... 生成展示收敛盆地的精美分形图案
```

## C++ 依赖

| 库 | 版本 | 用途 | 主页 |
|----|------|------|------|
| [doctest](https://github.com/doctest/doctest) | 2.4.12 | 快速 C++ 测试框架 | https://github.com/doctest/doctest |
| [spdlog](https://github.com/gabime/spdlog) | 1.17.0 | 快速 C++ 日志库 | https://github.com/gabime/spdlog |
| [cxxopts](https://github.com/jarro2783/cxxopts) | 3.3.1 | 命令行选项解析 | https://github.com/jarro2783/cxxopts |
| [Matplot++](https://github.com/alandefreitas/matplotplusplus) | 1.2.2 | C++ 数据可视化 | https://github.com/alandefreitas/matplotplusplus |
| [nanobind](https://github.com/wjakob/nanobind) | 2.6.1 | C++/Python 互操作（最小开销） | https://github.com/wjakob/nanobind |
| OpenMP | （系统） | 共享内存并行计算 | https://www.openmp.org/ |

## Python 依赖

| 包 | 用途 | 主页 |
|----|------|------|
| [NumPy](https://numpy.org/) | N 维数组、数值计算 | https://numpy.org/ |
| [SciPy](https://scipy.org/) | 科学算法（优化、插值） | https://scipy.org/ |
| [Matplotlib](https://matplotlib.org/) | 绘图与数据可视化 | https://matplotlib.org/ |
| [Numba](https://numba.pydata.org/) | JIT 编译加速 | https://numba.pydata.org/ |
| [Cython](https://cython.org/) | Python C 扩展 | https://cython.org/ |
| [PyQt6](https://www.riverbankcomputing.com/software/pyqt/) | GUI 支持 | https://www.riverbankcomputing.com/software/pyqt/ |
| [pytest](https://docs.pytest.org/) | 测试框架 | https://docs.pytest.org/ |
| [Loguru](https://github.com/Delgan/loguru) | 简化日志 | https://github.com/Delgan/loguru |

## 开发工具

| 工具 | 版本 | 用途 |
|------|------|------|
| [clang-format](https://clang.llvm.org/docs/ClangFormat.html) | 22+ | C++ 代码格式化（Microsoft 风格，100 列） |
| [clang-tidy](https://clang.llvm.org/extra/clang-tidy/) | 22+ | C++ 静态分析 |
| [ruff](https://docs.astral.sh/ruff/) | latest | Python lint 和格式化 |
| [pyright](https://github.com/microsoft/pyright) | latest | Python 类型检查 |

## 许可证

本项目使用 [MIT 许可证](LICENSE) 授权。

## 致谢

Kosy 基于以下优秀的开源项目构建，在此对它们的开发者和社区致以诚挚的感谢：

| 项目 | 说明 | 主页 |
|------|------|------|
| [CPM.cmake](https://github.com/cpm-cmake/CPM.cmake) | CMake 包管理器 | https://github.com/cpm-cmake/CPM.cmake |
| [doctest](https://github.com/doctest/doctest) | 快速 C++ 测试框架 | https://github.com/doctest/doctest |
| [spdlog](https://github.com/gabime/spdlog) | 快速 C++ 日志库 | https://github.com/gabime/spdlog |
| [cxxopts](https://github.com/jarro2783/cxxopts) | 命令行选项解析 | https://github.com/jarro2783/cxxopts |
| [Matplot++](https://github.com/alandefreitas/matplotplusplus) | C++ 数据可视化库 | https://github.com/alandefreitas/matplotplusplus |
| [nanobind](https://github.com/wjakob/nanobind) | 轻量级 C++/Python 互操作 | https://github.com/wjakob/nanobind |
| [NumPy](https://numpy.org/) | 科学计算基础包 | https://numpy.org/ |
| [Matplotlib](https://matplotlib.org/) | 综合绘图库 | https://matplotlib.org/ |
