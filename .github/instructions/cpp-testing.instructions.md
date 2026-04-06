---
applyTo: "**/*_test.cpp"
---
# doctest Guidelines

- Use `TEST_CASE` / `SUBCASE`; keep tests independent.
- Prefer `CHECK`, `REQUIRE`, and typed exception assertions.
- Use `doctest::Approx` for floating-point comparisons with explicit tolerances.
- Avoid shared mutable globals between tests.
- Link tests with `kosy_cxx_test` so doctest, cxxopts, and Matplot++ are available per project CMake.
