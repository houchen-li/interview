---
applyTo: "**/*.cpp,**/*.hpp"
---
# C++ Standards (Kosy)

- **Standard**: C++23; follow repository toolchain options.
- **Format**: clang-format Microsoft style (`.clang-format`).
- **Naming**: `camelCase` functions, `PascalCase` types, `m_` private members.
- **Trailing return types**: required for non-void functions and methods (`auto f() -> T`).
- **Errors**: use exceptions; avoid introducing alternate error-type frameworks in new code.
- **Tests**: doctest in `*_test.cpp`, registered via `kosy_cxx_test`.
- **Headers**: Doxygen file comment with `@copyright Copyright (c) YYYY Kosy Development Team` and `@license MIT`.
