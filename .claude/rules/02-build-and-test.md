# Build and Test

## Presets

| Preset | Description |
| ------ | ----------- |
| `linux-gcc-x64` | Linux, GCC, x86_64 |
| `linux-clang-x64` | Linux, Clang, x86_64 |
| `darwin-gcc-arm64` | macOS, GCC, arm64 |
| `darwin-clang-arm64` | macOS, Clang, arm64 |
| `windows-msvc-x64` | Windows, MSVC, x86_64 |

Build trees default to `out/build/<preset>`; install roots to `out/install/<preset>` per `CMakePresets.json`.

## Commands

```bash
# Configure
cmake --preset linux-clang-x64

# Build
cmake --build out/build/linux-clang-x64

# C++ tests (requires testing enabled at configure time)
ctest --test-dir out/build/linux-clang-x64
```

```bash
# Python
uv sync
uv run pytest
```

## CMake options (common)

- **`KOSY_BUILD_TESTING`**: enable Kosy test targets and CTest wiring
- **`BUILD_TESTING`**: CTest master switch; `kosy_cxx_test` requires both with testing on
- **`KOSY_ENABLE_INSTALL`**: install rules and versioning for libraries
- **`CMAKE_UNITY_BUILD`**, **`BUILD_SHARED_LIBS`**, **`CMAKE_CXX_EXTENSIONS`**: standard project toggles

## CMake functions

See `cmake/utils.cmake`:

- `kosy_cxx_library` — static or interface libraries with header file sets
- `kosy_cxx_module` — C++ modules (`FILE_SET CXX_MODULES`)
- `kosy_cxx_test` — doctest executable + `add_test`
