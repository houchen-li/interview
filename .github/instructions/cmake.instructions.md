---
applyTo: "**/CMakeLists.txt,**/*.cmake"
---
# CMake Guidelines

- Prefer **CMake Presets** over hand-rolled configure lines in docs.
- Fetch C++ libraries with **CPM** in `cmake/third_party`; centralize versions.
- Use helpers from `cmake/utils.cmake`:
  - `kosy_cxx_library` for libraries (headers via `FILE_SET HEADERS`, includes rooted at `src/`).
  - `kosy_cxx_module` for C++ module targets (`IXXS`).
  - `kosy_cxx_test` for doctest executables and `add_test` (requires `BUILD_TESTING` and `KOSY_BUILD_TESTING`).
- Expose aliases as `kosy::<target>` for consistency.
- Keep `KOSY_*` options meaningful: testing, install, soversion.
