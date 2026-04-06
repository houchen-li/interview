# Kosy — Copilot Instructions

Kosy is a **C++23** / **Python 3.14+** repository for math interview problems and experiments, licensed under **MIT**. Builds use **CMake** (Ninja, presets), **CPM** for C++ dependencies, and **uv** for Python. Interop uses **nanobind**.

## Quick reference

| Task | Command |
| ---- | ------- |
| Configure | `cmake --preset <preset>` |
| Build | `cmake --build out/build/<preset>` |
| CTest | `ctest --test-dir out/build/<preset>` |
| Python env | `uv sync` |
| Python tests | `uv run pytest` |
| Format C++ | `clang-format -i <files>` |
| Format/lint Python | `ruff format`, `ruff check --fix` |

Presets: `linux-gcc-x64`, `linux-clang-x64`, `darwin-gcc-arm64`, `darwin-clang-arm64`, `windows-msvc-x64`.

## Coding rules (summary)

- **C++**: Microsoft/clang-format style; **camelCase** functions; **`m_`** private members; **trailing return types required**; **exceptions** for errors; **doctest** for tests.
- **Python**: PEP 8 + **ruff** + **pyright**; **pytest** for tests.
- **Copyright**: Doxygen file header in C++ (`@copyright Copyright (c) YYYY Kosy Development Team`); Python first line `# Copyright (c) YYYY Kosy Development Team. All rights reserved.`
- **TODO**: `// TODO(github_id): …` or `# TODO(github_id): …`
- **Security**: no hardcoded secrets.

## Details

See `.github/instructions/` for scoped rules: workflow, C++ standards, CMake, Python, testing, copyright, security, and TODO format.
