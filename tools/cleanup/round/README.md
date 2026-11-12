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
$R check --native                                 # plus the native golden check (port build + scenario + state diff)
$R try --native -- <edit>                         # the same for one candidate edit, before it is reverted
$R golden status                                  # the native golden tool itself (no round needed)
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
  `$LC` label). A class rename counts too: its vtable's `.gnu.linkonce.d.<vtable>` section is compared under the
  new name when exactly one new section of the same kind has the same bytes, and the vtable symbol (with its config
  entry moved) goes through the same rename test. A relocation that now names a different existing symbol (another
  overload, another virtual) is a DIFF with class `retarget`, even though matchtool would still say MATCH.
- `DIFF`: the first differing section, the function and offset, about six instructions old | new with relocation
  targets, and matchtool's verdict for that function against the ELF.
- `COMPILE-FAIL`: the first compiler error lines.

Exit status: 0 identical or renamed, 1 difference or compile failure, 2 the edit did not apply.

## Native golden check (--native, `golden`)

The checks above prove the PS2 build is byte-identical. They cannot see the code the native port compiles instead:
`#else` / `!SLY2_EE_LAYOUT` branches, C fallbacks for asm, and type changes that move host layouts. `--native` (and
`round.py golden ...`, a pass-through to `tools/cleanup/native/golden.py`) builds the 64-bit port against the round's
tree and replays a fixed scenario against a golden state stream:

1. **port**: `git archive` of the port commit recorded in the golden into `<cache>/port` (file times = commit time;
   kept while the commit is the same). The port's `decomp/` is a mirror of the candidate's `include/ src/ config/`
   (a worktree's tracked and untracked files as they are on disk, or a commit); only files whose content changed are
   rewritten, so ninja rebuilds what changed.
2. **gen**: the port's generators, as after a submodule bump: gen_compat_asserts, gen_layout_maps, gen_overrides,
   `tools/state/gen_offsets.py --abi host`. Generated files whose content did not change keep their file times.
   **Stale hunks** (an optional port hunk no longer matches: the decomp changed under a port workaround) and
   **missing hunks** (a required one no longer matches: usually a landed port request, sometimes a change that broke
   the workaround) are reported, not fatal; the build goes on without them.
3. **build**: cmake (Debug, clang, tests off), ninja, gen_sym_aliases (its AST cache entries are dropped for every
   TU whose ninja dependencies changed), ninja sly2 (includes the port's EE-asm lint).
4. **run**: `sly2 --load-level <Paris> --frames N` with `SLY2_DT_REPLAY=fixed` (virtual 1/60 s clock),
   `SLY2_UPDATE_SCREENS=1`, ASLR off, a fixed `SLY2_PAD_SCRIPT` and the per-frame state stream of the port's
   `tools/state` (schema v2: clock, pad, Sly's state / position / velocity, camera, save blob, object lists).
5. **diff**: the port's `tools/state/diff.py` against `<golden>/stream.jsonl`, schema tolerances, aligned by frame.

Scenarios (in golden.py; the golden stores the one it was recorded with and `check` replays that one):

| scenario | frames | inputs | covers |
|---|---|---|---|
| default | 5300 | CROSS at 1395 (intro select); from 4400 run, jump + double jump, slow walk, run right + jump, run left | the intro relay (walk, pipe jump ~1511, climb, roof, binocucom), pad control from ~4000, running / jumping, a fall into the water |
| `--long` | 12000 | the same intro, then boot-15's soak loop P3 looped from read 4400 (`SLY2_PAD_LOOP=4400:1800`) | more ground and guards; reaches the guardbhv.cpp patrol path (removing the port's VuScaleByX hunk crashes it at frame 5263) |

**Output** (at most about ten lines; details in `<cache>/logs/`: gen_overrides.log, build.log, run.log, diff.txt
with the full per-field and per-class tables, diff.json):

```
native golden: /home/claude/work/wt-ngdemo @ 9dd812c3fa + uncommitted changes vs golden integrate/decomp-v1.29 (9dd812c3fa) (port ce8e58e2eb, scenario default v1, 5300 frames)
  RESULT     FAIL  diverged
  first divergence: frame 725: field jt.pos, a [4788.34, -6845.09, 1306.73], b [4788.34, -6845.09, 1306.16]
  field                first@   div/of      max err  golden | candidate at first
  jt.pos                  725  4072/5300       15.9  [4788.34, -6845.09, 1306.73] | [4788.34, -6845.09, 1306.16]
  jt.vel                  725  1108/5300        541  [-1639.58, 226.873, 1884.2] | [-1639.58, 226.873, 1848.81]
  rand.seed               737  4562/5300   4.18e+09  100060067 | 3346240277
  objects: 11 of 89 classes differ (first at frame 900, cid 22)
  time: port 0s, decomp 0s, gen 6s, build 35s, run 6s, diff 2s; total 48s
```

(That is `round.py try --native` of `0.5f` -> `0.49f` in the C fallback of air.cpp's jump-to-target: the PS2 object
is IDENTICAL, the native launch velocity is not.) How to read it:

- `RESULT PASS`: every frame equal within the schema tolerances (in practice bit-identical: the run is deterministic).
- `RESULT FAIL diverged`: the first frame and field that differ, then the diverged fields, earliest first: frames
  diverged / compared, max error, both values at the first divergence. Read the earliest field first; later ones are
  usually consequences (`rand.seed` diverging means the game took another branch that draws random numbers).
  Frame k is the record taken at the start of harness frame k (docs/state.md in the port).
- `RESULT FAIL harness killed by SIGSEGV after N records` / `exit 139`: a crash; the backtrace frames follow when the
  port's signal handler ran (none for a stack overflow). Frames up to N are still diffed.
- `RESULT FAIL (build)`: gen / compile / link failed: the first errors and the failed targets. If the failed TUs are
  the ones listed under missing hunks, the decomp changed lines the port patches: the port needs a bump to this decomp
  (port work), not necessarily a decomp fix.
- `RESULT BUILD-ONLY`: `SLY2_ISO` is not set; the build ran, the scenario did not.
- `stale hunks` / `missing hunks`: informative; the list is in `logs/gen_overrides.log`. A new stale hunk after an edit
  means the edit touched code the port works around: tell the port track.
- `note: host offsets ... differ from the golden's`: a type change moved a schema field natively (fields are still
  compared by name).

Exit status: 0 PASS / BUILD-ONLY, 1 FAIL, 2 setup error (no golden, cache locked, < 2 GB free). With `check --native`
a native FAIL makes the check fail; with `try --native` it makes the verdict a failure (`--keep` then does not keep).

**The port window.** The port's generators and hunks follow one decomp version: a port commit builds the decomp it
pins and, usually, small rounds on top of it. It cannot build older versions (renamed classes, declarations that
moved decomp-side) and needs a bump for newer ones that change the lines it patches. The golden therefore records its
port commit, and `check` always builds that one (`check --port REF` builds another and flags the report: a
cross-port comparison, where differences may come from the port). Run `--native` in rounds whose base is the golden's
decomp version; after an integration tag, re-baseline once the port has bumped to it.

**Recording and re-baselining.** Record from a known-good decomp commit and the port commit that pins it:

```sh
round.py golden record --decomp integrate/decomp-v1.29 --port <port commit> [--force]   # default scenario
round.py golden record --long --decomp integrate/decomp-v1.29 --port <port commit> [--force]
round.py golden status [--long]
```

`record` needs `SLY2_ISO`, refuses uncommitted trees, runs the scenario twice and writes the golden only if both runs
are identical. `<golden>/meta.json` records the decomp commit, the port commit and ref, the scenario, the stale /
missing hunk counts and the end state; `offsets_host.json` and `stale_hunks.txt` sit next to the stream. Re-record
(`--force`) after (a) a port bump: new port commit, new decomp tag; (b) an intentional behaviour change in the decomp
(a native fix that makes the port more faithful): check first, confirm in diff.txt that only the intended frames
and fields moved, then record from the commit that has the change and say so in its message; (c) a scenario change
in golden.py (bump its version). The golden is a regression reference, not a fidelity oracle: "same as the golden"
is not "same as PCSX2" (docs/state.md in the port for the PCSX2 side).

Current golden (2026-11-12): decomp `integrate/decomp-v1.29` (9dd812c3f) + port `claude/boot-15` at ce8e58e2e
(decomp pin v1.29), both scenarios.

Settings for the native check (environment variable, then the `[native]` section of the round config, then the
default): `cache` SLY2_NATIVE_CACHE (`{scratch}/native-golden/cache`: the port copy, its build directory, logs; about
140 MB), `golden` SLY2_NATIVE_GOLDEN (`{scratch}/native-golden/golden`; `-long` appended for `--long`), `port`
SLY2_PORT_REPO (`/home/claude/work/sly2-port`), `port_ref` SLY2_PORT_REF (`claude/boot-15`, record's default
`--port`), `iso` SLY2_ISO (unset: build only), `prefix` SLY2_DEPS_PREFIX (`/home/claude/work/deps/prefix`), `jobs` JOBS.

Timings on two cores: a new port commit (cold cache) about 2.5 minutes (most of it gen_sym_aliases' AST pass); a
one-file edit about 50 seconds (gen 6 s, build 35 s, run 6 s, diff 2 s); `--long` adds about 12 seconds of run; a
switch between decomp versions that change many headers about 2 minutes. One golden.py run at a time per cache (a
lock file).

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
