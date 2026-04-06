---
applyTo: "**/*_test.py,**/test_*.py"
---
# pytest Guidelines

- Use **pytest** only; run with `uv run pytest`.
- Name tests `test_*` with clear scenarios; avoid redundant docstrings.
- Use fixtures in `conftest.py` for shared setup; choose minimal scope.
- Mock external I/O and clocks; do not mock the unit under test.
- Use `pytest.raises` with `match=` for exception text when stable.
