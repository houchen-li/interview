# Kosy — Copilot Instructions

Kosy is a **C++23** / **Python 3.14+** repository for math interview problems and experiments, licensed under **MIT**. Builds use **CMake** (Ninja, presets), **CPM** for C++ dependencies, and **uv** for Python. Interop uses **nanobind**.

## Quick reference

| Task               | Command                               |
| ------------------ | ------------------------------------- |
| Configure          | `cmake --preset <preset>`             |
| Build              | `cmake --build out/build/<preset>`    |
| CTest              | `ctest --test-dir out/build/<preset>` |
| Python env         | `uv sync`                             |
| Python tests       | `uv run pytest`                       |
| Format C++         | `clang-format -i <files>`             |
| Format/lint Python | `ruff format`, `ruff check --fix`     |

Presets: `linux-gcc-x64`, `linux-clang-x64`, `darwin-gcc-arm64`, `darwin-clang-arm64`, `windows-msvc-x64`.

## Coding rules (summary)

- **C++**: Microsoft/clang-format style; **camelCase** functions; **`m_`** private members; **trailing return types required**; **exceptions** for errors; **doctest** for tests.
- **Python**: PEP 8 + **ruff** + **pyright**; **pytest** for tests.
- **Copyright**: Doxygen file header in C++ (`@copyright Copyright (c) YYYY Kosy Development Team`); Python first line `# Copyright (c) YYYY Kosy Development Team. All rights reserved.`
- **TODO**: `// TODO(github_id): …` or `# TODO(github_id): …`
- **Security**: no hardcoded secrets.

## Details

See `.github/instructions/` for scoped rules: workflow, C++ standards, CMake, Python, testing, copyright, security, and TODO format.

---

# andrej-karpathy-skills

Behavioral guidelines to reduce common LLM coding mistakes. Merge with project-specific instructions as needed.

**Tradeoff:** These guidelines bias toward caution over speed. For trivial tasks, use judgment.

## 1. Think Before Coding

**Don't assume. Don't hide confusion. Surface tradeoffs.**

Before implementing:

- State your assumptions explicitly. If uncertain, ask.
- If multiple interpretations exist, present them - don't pick silently.
- If a simpler approach exists, say so. Push back when warranted.
- If something is unclear, stop. Name what's confusing. Ask.

## 2. Simplicity First

**Minimum code that solves the problem. Nothing speculative.**

- No features beyond what was asked.
- No abstractions for single-use code.
- No "flexibility" or "configurability" that wasn't requested.
- No error handling for impossible scenarios.
- If you write 200 lines and it could be 50, rewrite it.

Ask yourself: "Would a senior engineer say this is overcomplicated?" If yes, simplify.

## 3. Surgical Changes

**Touch only what you must. Clean up only your own mess.**

When editing existing code:

- Don't "improve" adjacent code, comments, or formatting.
- Don't refactor things that aren't broken.
- Match existing style, even if you'd do it differently.
- If you notice unrelated dead code, mention it - don't delete it.

When your changes create orphans:

- Remove imports/variables/functions that YOUR changes made unused.
- Don't remove pre-existing dead code unless asked.

The test: Every changed line should trace directly to the user's request.

## 4. Goal-Driven Execution

**Define success criteria. Loop until verified.**

Transform tasks into verifiable goals:

- "Add validation" → "Write tests for invalid inputs, then make them pass"
- "Fix the bug" → "Write a test that reproduces it, then make it pass"
- "Refactor X" → "Ensure tests pass before and after"

For multi-step tasks, state a brief plan:

```
1. [Step] → verify: [check]
2. [Step] → verify: [check]
3. [Step] → verify: [check]
```

Strong success criteria let you loop independently. Weak criteria ("make it work") require constant clarification.

---

**These guidelines are working if:** fewer unnecessary changes in diffs, fewer rewrites due to overcomplication, and clarifying questions come before implementation rather than after mistakes.
