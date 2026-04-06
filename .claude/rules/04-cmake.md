# CMake Conventions

## CPM

Third-party packages are declared under `cmake/third_party` using **CPM**. Keep versions centralized; reuse `CPM_SOURCE_CACHE` (`third_party/`).

## Helpers (`cmake/utils.cmake`)

### `kosy_cxx_library`

Creates a target with optional install, `FILE_SET HEADERS`, include roots under `src/`, and alias `kosy::<name>`.

Arguments include `NAME`, `SRCS`, `HDRS`, `DEPS`, `COPTS`, `DEFINES`, `LINKOPTS`, and flags `PUBLIC`, `TESTONLY`, `DISABLE_INSTALL`.

### `kosy_cxx_module`

Declares a C++ module library using `IXXS` for module interface units.

### `kosy_cxx_test`

Builds a test executable linking doctest, cxxopts, Matplot++, and your `DEPS`; registers with CTest when `BUILD_TESTING` and `KOSY_BUILD_TESTING` are on.

## Presets

Use `CMakePresets.json` for configure/build/test presets instead of ad-hoc generator flags in documentation.
