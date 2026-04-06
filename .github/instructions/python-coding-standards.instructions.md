---
applyTo: "**/*.py,**/*.pyi"
---
# Python Standards

- **Style**: PEP 8; **ruff** is the enforced formatter/linter (`pyproject.toml`).
- **Types**: annotate public functions; use modern syntax (`list[str]`, `X | None`).
- **Imports**: stdlib → third party → local, separated by blank lines.
- **Errors**: specific exceptions; chain with `from` when re-wrapping.
- **Deps**: declare in `pyproject.toml`; sync with **uv**; commit `uv.lock` when dependencies change intentionally.
- **Package install**: `uv sync` installs dependencies and builds the nanobind extension `_core.*.so` via scikit-build-core (since `[tool.uv] package = true`).
- **Tools**: `uv run ruff`, `uv run pyright`, `uv run pytest`.
- **Copyright**: `# Copyright (c) YYYY Kosy Development Team. All rights reserved.`
