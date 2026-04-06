# Python Conventions

## Style

- **PEP 8** baseline; **ruff** is authoritative for formatting and many lint rules (`pyproject.toml`)
- **pyright** for static typing

## Types

- Annotate public APIs; use `|` for unions, generics from `collections.abc` or builtins

## Imports

Standard library → third party → local, blank line between groups.

## Tests

- **pytest** with descriptive `test_*` names
- Run via `uv run pytest`

## Project deps

- Declare in `pyproject.toml`; lock with **uv** (`uv.lock`)

## Tools

```bash
uv run ruff check .
uv run ruff format .
uv run pyright
uv run pytest
```
