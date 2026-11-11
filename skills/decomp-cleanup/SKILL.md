---
name: decomp-cleanup
description: Clean up a matching PS2 (g++ 2.95, EE) decomp into named, documented, view-unified source without losing the match.
---

# Decomp cleanup

Use this when a decomp already matches (every function byte-identical under a per-function checker) and the goal is
readable source: names, doc comments, one file per original file, shared types instead of per-function views. The
reasoning behind each step, and what went wrong without it, is in
[docs/guides/cleanup-playbook.md](../../docs/guides/cleanup-playbook.md). Do not use it to get functions matching in
the first place.

Tool names below are this repository's; on a new decomp copy tools/cleanup/ and adapt the paths (each script's
docstring lists its options).

## Rules for every phase

- Every edit keeps the match. A change that does not match is reverted, kept as a documented island, or timeboxed.
- MATCH is not enough: compare objects against a baseline built before the edit (symbols, sizes, sections,
  relocations by symbol), and run the full link before merging.
- One worktree and one scratch directory per agent; never `git stash`; commit after every file; keep a progress log
  in scratch that a resumed agent can follow.
- Linker symbols are renamed only with the rename tool, never by hand.
- In SDK C files never declare a C library or builtin name (`fabsf`, `isnan`, `scalbn`, ...): gcc inlines it. Keep
  the old identifier with an `__asm__` label for the new symbol, at declarations and call sites, and run the
  platform-only (Docker SDK) builds before calling the rename done.
- Game data (ELF, RAM dumps, save states, extracted assets) never goes in git; tools take it as paths.

## Phases

| phase | entry | work | exit |
|---|---|---|---|
| 0 prerequisites | functions match | matchtool-style checker; real full link; one verify script; environment note | verify passes on the untouched tree; progress leaves matched.csv unchanged |
| 1 names | phase 0 | per subsystem: rename functions (rename.py), fields (fieldrename.py, one header per batch), locals; doc comment per function, header per file, `(inferred)` markers | readability.py trend flat; docnames check 0; verify passes |
| 2 TU merge | names stable in the files touched | concatenate per-function files verbatim, then reconcile; pin asm functions with `#line` | every merged file all MATCH; link 0 differing bytes |
| 3 view unification | TUs merged; census run | rounds per type family, each family owning its headers; islands timeboxed (~20 min) and logged with a cause class | census tiers for the family empty or explained; island log complete |
| 4 island review | all family rounds integrated | group islands by root cause, fix causes not instances | each island fixed or documented as matching-only |
| 5 signatures | integration done | cross-family virtual/constructor signature changes in lockstep | no slot moved (reloc + nm diff 0) |
| 6 sweeps | stable base | scripted mechanical edits (vtable-entry copies, wrong-typed externs, repeated helpers) with skip lists | re-run on the final base, not merged |
| 7 integrate | family branches verified | one integration branch, merge in order, reconcile duplicates, checkpoint | full verify clean; bundle to the maintainer |
| side: snapshots | RAM snapshots of the levels in question | object discovery (vptr at +0xC), field typing across instances, name checks (tools/cleanup/snapshots/) | field types / names confirmed against the code that uses them; code wins on disagreement |
| side: 64-bit | views unified as far as they go | views to real classes with tools/cleanup/views (inventory with compiler layouts, propagation from annotated context records, rewrite kept only if byte-identical); exact danger test (tools/cleanup/ptr64/dt.py + judge.py, EE long = 8, accessed fields only); derive views from the real base; type pointer slots in pads; record pointer-free judgements in evidence.json | 0 dangerous in the milestone's files, unverifiable ones listed |

## Verification commands

```sh
# once per base, in a separate worktree at the base commit, BEFORE edits
tools/cleanup/objcmp.py record --root ../base-worktree base.json $(find src -name '*.cpp' -o -name '*.c')
# after each file or batch: matchtool + objcmp on the touched files and every includer of touched headers
tools/cleanup/check.sh base.json $(git diff --name-only BASE -- src include)
# before merging / at checkpoints
rm -f build/progress_cache.json && python3 tools/progress.py && git diff --quiet config/matched.csv
python3 tools/link.py            # 0 differing bytes, print the per-section table
python3 tools/linkcheck.py       # 0 broken
python3 tools/cleanup/docnames.py    # 0 unexplained misses
tools/verify.sh                  # all of the above in one summary
# census for view unification (re-run after each integration)
python3 tools/cleanup/census/dump_offsets.py && python3 tools/cleanup/census/census_all.py --config census.json
# snapshots (RAM images in build/snapshots/ram; rerun order in tools/cleanup/snapshots/README.md)
export SNAP_ELF=<ELF>   # or --elf on each script
python3 -I tools/cleanup/snapshots/snapshot_objects.py && python3 -I tools/cleanup/snapshots/infer_layout.py
python3 -I tools/cleanup/snapshots/namecheck.py all
# 64-bit danger test
python3 -I tools/cleanup/ptr64/dt.py . facts.json && python3 -I tools/cleanup/ptr64/judge.py facts.json judged.json
```

Snapshot caveats: they type fields and list live classes well, but bound a view's class only as "is-a" (prototypes,
declared types), and layout matching can spread one wrong slot. Behaviour objects exist only while pushed, so capture
states inside the situation. A cluster of name mismatches is usually a sampling artifact, not drift: read the code
before moving offsets.

## Agent prompt template (one family, one round)

Fill the angle-bracket fields; keep the rest verbatim.

```text
Read <workspace note> first. Env: <exports>. No sub-agents, nothing pushed, NEVER git stash (shared across
worktrees), git identity <name> <email>, every commit message ends with <trailer lines>. Worktree <path>, branch
<branch> from <base commit>. Scratch only in <scratch>/<family>/.
PROGRESS SAFETY: commit after EVERY file (or every unified view in a large file); never hold more than ~20 minutes of
uncommitted work; append what is done/next to <scratch>/<family>/PROGRESS.md after each commit. If resumed: read
PROGRESS.md, git status, git log; finish or revert the half-done edit (matchtool decides); continue.
CONTEXT: read docs/decisions.md section 1 and docs/guides/tu-merging.md "Views unified" (separate structs where views
really conflict; derived structs for supersets; unions only where a kind/mode field selects bytes or the original
had one representation; islands only after the timebox), and the naming conventions.
TASK: unify the private views of YOUR FAMILY's types (<family>: types <types>, headers <headers>; census list
<census tier file filtered to the family>). For each view: (1) complete the shared type (fields at the right offsets,
offset static-asserts, evidence in the header comment); (2) replace the view with the shared type, deleting it; keep a
cast only where the code really reinterprets memory; (3) check: tools/cleanup/check.sh <baseline> <files>. If the
replacement changes codegen, try the supported representations for up to ~20 minutes, else keep the view renamed
<Type>Island with a one-line reason and log it in <island log> (family, file, function, view, cause class, what was
tried; classes: register-alloc, scheduling, address-folding, type-width/signedness, aliasing, header-clash, other).
Prioritize by number of copies. You own ONLY your family's headers; for a field in another family's header use a local
derived struct or note it for later. Signature changes that span families go in your final report, not in code.
MEASURE before/after: view count for your family (census), struct definitions in source files, git diff --stat.
Timebox <hours>. END: check.sh on every touched file and all includers of changed headers (0 diffs); progress from a
deleted cache with matched.csv unchanged; link 0 differing bytes; linkcheck 0; docnames 0.
Final reply <= 6 lines: views removed / kept as islands, defs before -> after, header changes, cross-family items, head.
```

Other phases use the same skeleton with a different TASK and END: naming rounds (rename.py / fieldrename.py, doc
comments, readability.py before/after), sweeps (the script, its skip list, re-run on the new base), the island review
(group by cause, deep-dive, commit whole-group fixes only, report), integration (merge order, reconcile duplicates,
full verify, bundle).

## Coordinator checklist

- Keep a queue file: running agents, branches and heads, decisions, next steps; update it on every status change.
- Before launching a wave, record the baseline once and give every agent the same baseline path.
- Re-run the agent's verification yourself before merging; run the whole-tree gates at each checkpoint.
- Under quota pressure run one or two agents; park branches at clean commits and resume them one at a time.
- Surface maintainer decisions (config aliases, representation choices, renames to review) as soon as they appear.
- With a second team (the port, its own repository), coordinate through a message board outside both repos: one file
  per request, one writer per log file.
