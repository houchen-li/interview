# C++ Coding Style

## Formatting

- **clang-format** (Microsoft base style, repository `.clang-format`)
- **clang-tidy** for static analysis where enabled

## Naming

- Functions: **camelCase**
- Types: **PascalCase**
- Private members: **`m_` prefix**
- Namespaces: lowercase, short

## Trailing return types

**Required** for functions and methods (non-void):

```cpp
auto solve(const Matrix& a) -> Vector;
```

## Class design

- Explicit constructors when conversion should not be implicit
- Const-correct APIs
- Value semantics by default; smart pointers when shared ownership is real

## Modern C++

- `enum class`, `nullptr`, standard containers and algorithms
- Prefer exceptions over ad-hoc error codes for exceptional paths

## Comments

- Doxygen file headers with `@copyright Copyright (c) YYYY Kosy Development Team`
- Explain non-obvious rationale and invariants
