# Development Workflow

## Steps

1. **Implement** in the correct layer (`src/`, Python packages as established).
2. **Test**: doctest for C++, pytest for Python.
3. **Build**: `cmake --preset …` and `cmake --build …`.
4. **Format**: `clang-format -i` on C++ files; `ruff format` / `ruff check --fix` on Python.
5. **Lint**: `clang-tidy` (C++), `ruff` + `pyright` (Python).
6. Ship when green locally (and in CI if applicable).

## C++ path

```bash
cmake --preset linux-clang-x64 -DKOSY_BUILD_TESTING=ON -DBUILD_TESTING=ON
cmake --build out/build/linux-clang-x64
ctest --test-dir out/build/linux-clang-x64
```

## Python path

```bash
uv sync
uv run pytest
```

## File creation

- No unsolicited demos, samples, or documentation files
- Dependencies: C++ via CPM/CMake; Python via `pyproject.toml` + uv

## Documentation policy

- Prefer tests and clear code over new Markdown unless requested
