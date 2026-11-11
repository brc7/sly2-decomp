# Round harness (round.py)

One command each to set up a cleanup round, check it, and try a candidate edit. It replaces the per-round scratch
scripts earlier rounds copied and adapted (a base worktree, `rec.sh`, `chk.sh`, `secdiff.py`, `fullcheck.sh`,
`symcheck.py`).

```sh
R=tools/cleanup/round/round.py
$R start views7 --base integrate/decomp-v1.20     # worktree ../wt-views7 on branch round/views7, baseline recorded
cd ../wt-views7
$R try fix.patch                                  # apply, check, revert: IDENTICAL / RENAMED / first difference
$R try -- sed -i 's/\bgDiv\b/divisor/g' src/game/core/math/mat.cpp
git diff | $R try --stdin --keep                  # keep the edit when it is identical
$R check                                          # incremental check of everything changed since the baseline
$R check --full                                   # plus symcheck, linkcheck, docnames, progress, real link
$R status
```

The round is found from `--round NAME`, `$SLY2_ROUND`, or the current directory (any directory inside the round's
worktree). Every command prints at most about ten lines; details go to `<scratch>/<NAME>/logs/`.

## Commands

**start NAME [--base REF] [--branch B] [--worktree DIR] [--reuse] [--no-progress]** creates the worktree (default
`<worktrees>/wt-NAME`) and branch (default `round/NAME`) at REF, symlinks the untracked inputs (disc image,
compilers, wibo), compiles every source into the shared object cache, and records the baseline in
`<scratch>/NAME/`: `baseobj/` (hard links to the baseline objects), `base.json` (objcmp.py views, for
tools/cleanup/check.sh), `symcheck.base.json`, `progress.base.log` and `progress_cache.base.json`, and `round.json`
(content keys of every source, base commit, config names). The tree must be clean. About 10 minutes on two cores
when the cache is cold (compiles plus a progress run); a second round on an already cached base needs only the
progress run (or seconds with `--no-progress`). `--reuse` adopts an existing worktree and re-records its baseline.

**check [--full]** is incremental by default. Every source has a content key: a hash of the source, every header it
includes (transitively, resolved the way the include lines name them), the matching tools and the compile
environment. Sources whose key differs from the baseline are the edited ones plus every includer of an edited
header; only those are compiled (or taken from the cache), and each object is compared with its baseline object
byte for byte (objdiff.py: every allocated section, relocations by target symbol, named symbols with value, size
and section). Then matchtool's comparison runs on every function and data symbol of those objects. `--full` adds:

| check | passes when |
|---|---|
| symcheck | no unresolved undefined symbol, duplicate strong definition or namespace-mangled definition that the baseline did not have (symcheck.py, from the cached objects) |
| linkcheck | tools/linkcheck.py exits 0 |
| docnames | tools/cleanup/docnames.py exits 0 |
| progress | tools/progress.py from a deleted cache prints N/N, leaves config/matched.csv unchanged, and no more sources fail to build than in the baseline (progress.py exits 1 whenever any source does not build, e.g. the SDK files that need Docker, so its exit status is not used). If more fail, one more run retries only the failed builds (a wibo timeout under load is transient; seen once in testing) |
| link | tools/link.py: 0 ld.lld errors, 0 differing bytes, every symbol at its config address |

The full run takes about as long as progress plus the link (14 minutes on two shared cores in testing; a retry of
failed builds adds about 7 more, most of it the SDK files failing again).

**try PATCHFILE | --stdin | -- COMMAND... [--keep] [--all] [--vs pre|base] [-p N]** checks one candidate edit:

1. snapshots the worktree as a git tree object through a temporary index (tracked and untracked files; the real
   index, the stash and the branches are never touched) and writes `<scratch>/NAME/try.pending.json`;
2. applies the edit: `git apply -pN` (atomic: a patch that does not apply changes nothing), or runs COMMAND in the
   worktree;
3. compiles the sources whose content key changed (the edited sources first, then includers), compares each with
   its object from before the edit (`--vs base`: with the baseline object) and stops at the first difference
   (`--all` checks every affected source);
4. restores the snapshot (only the files that differ are rewritten, then the tree is re-hashed to prove it), unless
   `--keep` and the result is not a difference. Ctrl-C and SIGTERM restore too; if the process is killed outright,
   the next round.py command for the round restores the snapshot from the pending file before doing anything else.

Output is one verdict line plus a few lines of detail:

- `IDENTICAL`: every affected object is byte-identical.
- `RENAMED`: the bytes and relocation positions are identical and only symbol names differ through a one-to-one
  map whose new names are either the old name's config entry moved (rename.py) or unknown before (a renumbered
  `$LC` label). A relocation that now names a different existing symbol (another overload, another virtual) is a
  DIFF with class `retarget`, even though matchtool would still say MATCH.
- `DIFF`: the first differing section, the function and offset, about six instructions old | new with relocation
  targets, and matchtool's verdict for that function against the ELF.
- `COMPILE-FAIL`: the first compiler error lines.

Exit status: 0 identical or renamed, 1 difference or compile failure, 2 the edit did not apply.

## Settings

Environment variable, then the config file (`$SLY2_ROUND_CONF`, default `~/.config/sly2/round.conf`, section
`[round]`, keys as in the first column), then the default:

| key | variable | default |
|---|---|---|
| scratch | SLY2_ROUND_SCRATCH | /home/claude/work/scratch |
| objcache | SLY2_ROUND_OBJCACHE | {scratch}/.objcache |
| worktrees | SLY2_ROUND_WORKTREES | the main repository's parent directory |
| disc | SLY2_ROUND_DISC | /home/claude/vclone/disc/SCUS_973.16 |
| compilers | SLY2_ROUND_COMPILERS | /home/claude/vclone/tools/compilers |
| wibo | SLY2_ROUND_WIBO | /home/claude/vclone/tools/wibo-i686 |
| pythonpath | SLY2_PYTHONPATH | /home/claude/work/pystub (rabbitizer for matchtool) |
| m2c_inc | SLY2_M2C_INC | /home/claude/m2c |
| jobs | JOBS | 2 |

```ini
[round]
scratch = /Users/me/sly2-scratch
disc = /Users/me/games/SCUS_973.16
compilers = /Users/me/sly2/tools/compilers
wibo = /Users/me/sly2/tools/wibo-macos
```

## Notes

- The object cache is shared by all rounds and keyed by content, so rounds on the same base reuse each other's
  objects. It only grows; delete `{objcache}` when no round is running (baselines keep their own hard links).
  A compile failure is cached too (`.fail` with the compiler output), except timeouts.
- matchtool locates the compilers, ELF and config from its own path, so round.py imports the round worktree's
  tools/matchtool.py; round.py itself can run from any checkout.
- One round.py command per round at a time (a lock file in the scratch directory): a `try` during a `check --full`
  would corrupt the progress run.
- The 329 SDK sources that need the Linux i386 compilers (Docker) do not build natively; they fail before and after
  and are reported as `both-fail`, as in tools/link.py. Run `tools/dev.sh progress` for them when SDK files change.
- Sources: objdiff.py (object comparison and first-difference listing), symcheck.py (the native-link symbol check).
