---
applyTo: "**/*"
---
# Workflow

## Implement → test → format → lint

1. Implement changes in `src/` (C++) or established Python paths.
2. Add/update tests: **doctest** (`*_test.cpp`) and **pytest**.
3. Configure with **CMake Presets**: `cmake --preset <name>`, then `cmake --build out/build/<name>`.
4. Run **CTest** when C++ tests are enabled: `ctest --test-dir out/build/<name>` (requires `KOSY_BUILD_TESTING` / `BUILD_TESTING` as appropriate).
5. Python: `uv sync` then `uv run pytest`.
6. Format: **clang-format** on C++; **ruff** on Python.
7. Lint: **clang-tidy** (C++), **ruff** + **pyright** (Python).

## Dependencies

- C++: **CPM** declarations in `cmake/third_party`; cache under `third_party/`.
- Python: `pyproject.toml` + **uv** (`uv.lock`).

## Documentation

Do not add unsolicited README or doc files; follow project policy in `AGENTS.md`.
