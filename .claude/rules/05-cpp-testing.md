# C++ Testing (doctest)

## Basics

- Tests live in `*_test.cpp` files (typical convention) and are wired with `kosy_cxx_test`.
- Use `TEST_CASE` and nested `SUBCASE` for clarity.
- Prefer `CHECK`/`REQUIRE` and typed exception checks (`CHECK_THROWS_AS`).

## Independence

- No ordering assumptions between tests
- Avoid static mutable shared state

## Floating point

- Use `doctest::Approx` or small helpers with explicit tolerances

## Running

After configure with testing enabled:

```bash
ctest --test-dir out/build/<preset>
```
