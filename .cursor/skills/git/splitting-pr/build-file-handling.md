# xmake.lua File Handling

How to handle `xmake.lua` files when splitting PRs. The strategy depends on whether the target directory already has an `xmake.lua` on master.

**Quick check:**

```bash
git show origin/master:path/to/xmake.lua 2>/dev/null && echo "EXISTS" || echo "NEW"
```

---

## Scenario A: Adding to EXISTING Directory

The directory already has an `xmake.lua` on master. **KEEP all existing targets and ADD yours.**

### Steps

```bash
git show origin/master:path/to/xmake.lua > /tmp/existing_xmake.lua

# Append new target blocks to end of file
cat >> /tmp/existing_xmake.lua << 'EOF'

target("your_new_target")
    set_kind("static")
    add_files("your_new_target.cc")
    add_headerfiles("your_new_target.h")
    add_deps(...)
EOF

cp /tmp/existing_xmake.lua path/to/xmake.lua
```

### Correct vs Wrong

```lua
-- CORRECT: All existing targets preserved + new one added at end

-- === EXISTING TARGETS (from master) - DO NOT DELETE ===
target("pva_diff_calculator")
    -- ...

target("statistics_aggregator")
    -- ...

target("evaluator")
    -- ...

-- === NEW TARGET (added by this PR) ===
target("version_comparator")
    set_kind("static")
    add_files("version_comparator.cc")
    add_headerfiles("version_comparator.h")
    add_deps(...)
```

```lua
-- WRONG: Only new target — deletes all existing targets!
target("version_comparator")
    -- ...
```

---

## Scenario B: Creating NEW Directory

No `xmake.lua` exists on master. **Create a separate `xmake.lua` per split PR** with only the targets relevant to that PR's files.

### Rules

1. Each PR gets its own `xmake.lua` with only its targets
2. Dependencies can reference targets that don't exist yet — OK if the tree doesn't build yet
3. Goal is file-target consistency, not build-ability

### Steps

```bash
# Write xmake.lua content with only this PR's targets
cat > /tmp/prN_xmake.lua << 'XMAKEEOF'
target("target_for_this_pr")
    set_kind("static")
    add_files("target_for_this_pr.cc")
    add_headerfiles("target_for_this_pr.h")
    add_deps("other_target") -- OK - doesn't exist yet
XMAKEEOF

mkdir -p path/to/new_dir/
cp /tmp/prN_xmake.lua path/to/new_dir/xmake.lua
```

### Example: Splitting new evaluator into 2 PRs

Original `xmake.lua` has: `statistics_aggregator`, `statistics_aggregator_test`, `evaluator_flags`, `evaluator`, `evaluation_main`

**PR 1** — only statistics_aggregator targets:

```lua
target("statistics_aggregator")
    set_kind("static")
    add_files("statistics_aggregator.cc")
    add_headerfiles("statistics_aggregator.h")
    add_deps("pva_diff_calculator", "kosy::math_statistics")

target("statistics_aggregator_test")
    set_kind("binary")
    set_default(false)
    add_files("statistics_aggregator_test.cc")
    add_deps("statistics_aggregator")
    add_tests("default")
```

**PR 2** — evaluator targets (merged after PR 1):

```lua
target("evaluator_flags")
    set_kind("static")
    add_files("evaluator_flags.cc")
    add_headerfiles("evaluator_flags.h")
    add_packages("abseil")

target("evaluator")
    set_kind("static")
    add_files("evaluator.cc")
    add_headerfiles("evaluator.h")
    add_deps("pva_diff_calculator", "statistics_aggregator")

target("evaluation_main")
    set_kind("binary")
    add_files("evaluation_main.cc")
    add_deps("evaluator", "evaluator_flags")
```
