# AGENTS.md — Kosy Project Guide for AI Agents

## Project Overview

Kosy is a **C++23 / Python** hybrid repository focused on **math problems** useful for interview preparation and exploration. The project uses **nanobind** for C++/Python interop, **doctest** for C++ tests, and **pytest** for Python tests. Code is licensed under the **MIT License**.

## Build System

- **CMake** 3.31+ with **Ninja** as the primary generator (see `CMakePresets.json`).
- **CPM** downloads and pins C++ dependencies; sources are cached under `third_party/` (see `CPM_SOURCE_CACHE`).
- **vcpkg** is used where appropriate for native dependencies (project-local tree).
- **uv** manages Python dependencies from `pyproject.toml` and `uv.lock`.

### Key Commands

| Command | Purpose |
| ------- | ------- |
| `cmake --preset <name>` | Configure the build (e.g. `linux-clang-x64`) |
| `cmake --build out/build/<preset>` | Compile after configuration |
| `ctest --test-dir out/build/<preset>` | Run CTest-registered tests (requires testing enabled) |
| `uv sync` | Install/sync Python dependencies into the project environment |
| `uv run pytest` | Run Python tests |
| `clang-format -i <files>` | Format C++ sources (Microsoft style; project `.clang-format`) |
| `ruff check` / `ruff format` | Lint and format Python |

### CMake Presets

| Preset | Platform / toolchain |
| ------ | --------------------- |
| `linux-gcc-x64` | Linux, GCC, x86_64 |
| `linux-clang-x64` | Linux, Clang, x86_64 |
| `darwin-gcc-arm64` | macOS, GCC, Apple Silicon |
| `darwin-clang-arm64` | macOS, Clang, Apple Silicon |
| `windows-msvc-x64` | Windows, MSVC, x86_64 |

Binary and install roots default under `out/build/<preset>` and `out/install/<preset>` per `CMakePresets.json`.

### CMake Options (selected)

| Option | Role |
| ------ | ---- |
| `KOSY_BUILD_TESTING` | When `ON`, enables CTest and Kosy test targets (`OFF` by default in root `CMakeLists.txt`) |
| `KOSY_ENABLE_INSTALL` | Controls install rules and versioning properties for libraries |
| `BUILD_TESTING` | CTest standard flag; `kosy_cxx_test` requires both `BUILD_TESTING` and `KOSY_BUILD_TESTING` |
| `CMAKE_UNITY_BUILD` | Unity builds (`OFF` by default) |
| `BUILD_SHARED_LIBS` | Shared vs static libraries |
| `CMAKE_CXX_EXTENSIONS` | GNU C++ extensions (`ON` by default) |
| `KOSY_SOVERSION` | Shared library SOVERSION for installed targets |

### CMake Helper Functions (`cmake/utils.cmake`)

| Function | Purpose |
| -------- | ------- |
| `kosy_cxx_library` | Declares a static/interface library with header file sets, include paths under `src/`, optional install |
| `kosy_cxx_module` | Declares a C++ module library using `FILE_SET CXX_MODULES` |
| `kosy_cxx_test` | Declares a test executable linking **doctest**, **cxxopts**, **Matplot++**, registers with CTest |

Targets are also exposed as `kosy::<name>` aliases (see `cmake/utils.cmake`).

## Languages & Toolchains

| Language | Version / notes | Primary layout |
| -------- | ---------------- | -------------- |
| **C++** | C++23, GNU extensions allowed (`CMAKE_CXX_EXTENSIONS`) | `src/`, `tests/` |
| **Python** | 3.14+ | Scripts and bindings alongside C++ |
| **CMake** | 3.31+ | `CMakeLists.txt`, `cmake/*.cmake` |

## Code Formatting

Formatting is **mandatory** after substantive edits:

- **C++**: `clang-format` 22+ using the repository **Microsoft** base style (see `.clang-format`).
- **Python**: **ruff** (formatter + linter), aligned with `pyproject.toml`.

Run formatters on touched files before considering work complete.

## Repository Structure

```
kosy/
├── src/                 # C++ libraries, modules, and related sources
├── tests/               # C++ tests and test-only CMake
├── cmake/               # Toolchains, third-party (CPM), utilities, compile_commands helpers
├── third_party/         # CPM cache and vendored sources
├── out/                 # Build and install trees (preset-specific, typically gitignored)
├── CMakePresets.json    # Configure / build / test presets
├── CMakeLists.txt       # Root project
├── pyproject.toml       # Python project metadata and tool config
└── uv.lock              # Locked Python dependencies
```

## Development Workflow

1. Implement functionality (C++ under `src/`, Python alongside or in dedicated modules per existing layout).
2. Add or update tests: **doctest** for `*_test.cpp`, **pytest** for Python.
3. Configure and build: `cmake --preset <preset>` then `cmake --build out/build/<preset>`.
4. Run tests: `ctest --test-dir out/build/<preset>` and/or `uv run pytest`.
5. **Format**: `clang-format -i` on C++ changes; `ruff format` / `ruff check --fix` on Python.
6. **Lint**: `clang-tidy` on C++ where configured; `ruff` + **pyright** for Python.
7. Fix warnings; treat compiler warnings as errors where the project enables that.

## Copyright Header

C++ headers and sources use a **Doxygen** file header, for example:

```cpp
/*!
 * @file example.hpp
 * @brief Short description.
 * @author Kosy Development Team
 * @copyright Copyright (c) 2026 Kosy Development Team
 * @license MIT
 */
```

Python modules use:

```python
# Copyright (c) 2026 Kosy Development Team. All rights reserved.
```

Use the **current year** for new files. CMake script files and `.mdc` rule files do not require these headers (see project rules).

## TODO Format

```
// TODO(github_id): description [optional issue reference]
```

Use `#` instead of `//` in Python. Always include your GitHub ID in parentheses.

## File Creation Policy

- Do **not** add demos, samples, or unsolicited documentation files unless explicitly requested.
- Prefer extending existing modules and tests.
- Add C++ dependencies via **CPM** / CMake in `cmake/third_party` (or as established in-repo).
- Add Python dependencies in `pyproject.toml` and refresh the lockfile with **uv** as appropriate.
- Keep experimental or personal scratch work out of production paths unless the repo defines a dedicated area.
