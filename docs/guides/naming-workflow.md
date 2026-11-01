# Guide: naming functions (playbook, verification, the rename tool)

How unnamed functions (`func_XXXXXXXX`) get names, how much to trust a proposal, and how names are applied to the repo safely. Sources: the semantic playbook
(`reports/semantic/playbook.md`), the master-name conventions (`reports/master_names_v2/CONVENTIONS.txt`), four verifier rounds, the arbitration pass and the
rename tool (`reports/rename_tool/DESIGN.txt`). The current name list is `master_names_v2/master_names.csv` (6,186 rows); the generated per-subsystem tables
are in [reference/functions/](../reference/functions/).

## 1. Where things stand (master_names_v2)

| Region | Functions | Named in symbol_addrs | v2 tiers of the unnamed: T1 / T2 / T3 / NONE |
|---|---|---|---|
| crt0 thunks/factories 0x100000-0x1147D8 | 827 | 1 | 585 / 238 / 0 / 3 |
| core (util + Splice libs) | 521 | 66 | 390 / 65 / 0 / 0 |
| game TUs 0x12B3B0-0x2319E8 | 4,565 | 306 | 2,275 / 1,967 / 8 / 9 |
| SDK (989snd + Sony libs) | 705 | 379 | 305 / 21 / 0 / 0 |
| linkonce tail 0x254F00-0x259DE4 | 198 | 4 | 58 / 136 / 0 / 0 |
| **Total** | **6,816** | **756** | **3,613 / 2,427 / 8 / 12** |

T1 = commit-ready (60% of unnamed functions); T1 + T2 + T3 cover 99.8%. Pending, not yet in the CSV: `master_names_v2/PENDING_FIXES.txt` (EXIT/WARP corrections,
skipped class names, top-50 round merges) and the runtime6 renames (`IncrementSwNoPause`/`DecrementSwNoPause` 0x20F0C0/0x20F0F0, `StartStepplayerFollow` 0x142540).
The docs generator applies them as overlays (evidence `pending-fix` / `top50`).

## 2. Per-TU playbook (summary of `semantic/playbook.md`)

1. **Fix the TU boundary and identity first.** Every TU ends with `__static_initialization_and_destruction_0` + `_GLOBAL_$I` (`f(1, 0xFFFF)`); TUs with global destructors
   end with a third `_GLOBAL_$D` stub, which belongs to the TU before the next span (fixed in `tu_map.csv`). Guess the file name from **alphabetical link order** between
   known neighbours; do not trust the early Sly 1 majority label (wrong about a third of the time).
2. **Find the entry points**: callers outside the span (main 0x1A7518, ExecuteSwTransition 0x20E890, the world loader 0x13DA18, Splice thunks). Functions with no callers
   are vtable slots, startup-table entries, message hooks or callbacks.
3. **Map the Sly 1 counterpart by role, not by index**; order is largely kept, so a few pinned functions place the rest.
4. Per function, in order of yield:

| # | Step | Yield |
|---|---|---|
| 1 | Read the Sly 2 body and the matcher's header comment (ignore m2c extern blocks) | decisive ~60% |
| 2 | Name the callees (body-only call graph, not stale externs) | fixes the subsystem |
| 3 | Compare with the Sly 1 candidate **and its two neighbours** (shifts are the most common error) | catches alignment slips |
| 4 | Check the signature (arity, float vs pointer, setter vs getter) | cheap rejection |
| 5 | Constants and ELF data tables | often unique |
| 6 | Call-site arguments (`&D_002DDF40` = g_cm, `D_003DD490` = g_sw, ...) | resolves stubs and setters |
| 7 | Write name, signature, purpose, confidence H/M/L, evidence, and the confirming experiment | |

Function size and alignment confidence had **no** predictive value (20% vs 22% right). Confidence rubric: H = Sly 1 code matches or two independent signals agree;
M = role clear, name inferred; L = shape or callers only. When a name turns out wrong, check its neighbours: errors come in shifted runs.

## 3. Conventions (CONVENTIONS.txt, sections 1-7)

- **Class placeholders are decimal**: `CIDnn` / `Cidnn` with the decimal cid (cid 0x27 = `Cid39`); `BHVnnn`, `WIDGETnnn` likewise (CONFLICTS #11 came from mixing hex and decimal).
  A name whose only class identity is a placeholder is never T1 (except structural names, below).
- **Method names** in Sucker Punch verb+Class style: `LO::SetParent` -> `SetLoParent`, `X::LoadFromBrx` -> `LoadXFromBrx`, `X::PostLoad` -> `PostXLoad`; Hungarian roles ->
  `CLASS_Role` (`BENTLEY_FCanUseGadget`); real C++ classes -> `Class_Role` (`CRef_SetS32`), with the mangled gcc symbol in a separate column when the signature is confirmed.
- **Slot roles**: LO 3 HandleSpliceEvent, 4 SendMessage, 5 HandleMessage, 7 Init, 8 Add, 9 Remove, 10 SetParent, 11/12 Add/RemoveHierarchy, 14 LoadFromBrx, 15 PostLoad,
  16 Update, 19 RenderSelf, 22 OnRemove, 23 OnAdd, 40 CloneHierarchy, 41 Clone. **BHV slots** (r9 canon): 7 Init, 8 Start, 9 Close, 10 Update, 11 Update<Cls>Motion,
  12 GrfGroundProbe, 13 FHandleMessage, 15 Set<Cls>State.
- **Structural names are T1 by construction**: `StaticInit_<tu>`, `_GLOBAL_$I$<tu>`, `_GLOBAL_$D$<tu>`, `junk_XXXXXXXX`, `New<Cls>` factories and `Get<Cls>ClassDesc` getters
  (cross-checked against the descriptor table: allocation size 185/185, getter = vtable slot 6 199/199). Renaming a class renames these mechanically.
- **RefThunk names follow their native function** (T1 only if the native is T1/existing).
- **Class words**: BHV words accepted when runtime-confirmed or map confidence >= 0.6 (BHVGUARD, BHVANIM, BHVIDLE, BHVSTRIKE, BHVKO, BHVATTACK, BHVCOMBAT, BHVWALKPATH,
  BHVPATROL, BHVLEAP, BHVJUGGLED, BHVGOTO, BHVPATHLINK, BHVPATROLPATH; BHVHIT, BHVPATH, BHVRAIL, BHVPIPE); whole-game identities (GUARD, DIMITRI ... BOOTY, CONVENTIONS 7.11)
  count as identified because WE names and object strings back them. Coined words without a descriptor or string (RcCar, DirAnim, GuardMgr) force T2.
- **Uniqueness** against existing symbols by demangled base; collisions get an argument-type suffix, a class prefix, or `_<ADDR6>` (then T3).
- **Precedence** (highest first): arbitration > manual CONFLICTS fixes and the mesh sub-glob correction > BHV slot canon > verifier better names > runtime/parse-verified
  names > TU agents > v1 rows > unverified hints.
- **Tiers**: T1 = confidence >= 0.5, not a placeholder, not uncertain, not contradicted, not disputed; T2 = draft (placeholder, `?`, < 0.5, or no identifier); T3 = disputed
  (`disputed.csv`); confidence boosts: runtime +0.05, parse-verified floor 0.7, verifier CORRECT floor 0.85, cap 0.97.

## 4. Verification and calibration

**Independent verifiers** re-derive a sample of proposals from the code without seeing the proposer's evidence (80 rows per round, four rounds; plus a follow-up round):

| Proposer confidence | Strictly correct | Notes |
|---|---|---|
| >= 0.8 | 97-100% | errors are rare |
| 0.5-0.8 | 80-100% | errors were mostly class-identity slips |
| < 0.5 | name ~60%, purpose line ~85-95% | treat the name as a draft, the purpose as useful |

Signatures are a separate claim: 14 of 77 correct-or-plausible names from one batch carried a Sly 1 signature that contradicts Sly 2's arguments, so mangled names are
emitted only with a confident signature. Sly 1 alignment proposals (`~names`) were badly shifted in places and are never used unchecked.

**Stronger evidence types** (shown in the Evidence column of the generated tables):
- `runtime`: a fact about the function was observed in PCSX2 RAM (rounds 1-6, 62 runtime rows + 16 runtime-corrected in v2, plus later rounds); `runtime-corrected`
  means runtime changed the purpose (e.g. Bentley's "bomb" path is the crossbow dart).
- `parse`: the level parser followed this loader over 100% of the game's bytes (99 loader rows: every LO-derived level class). Parse verifies byte layouts, **not nouns**:
  placeholder-class loaders stay T2.
- `arbitration`: 67 decisions from disassembly and full reference scans (e.g. 0x20DF80 `SetSwTransition`, `HandleBasicSpliceEvent`/`HandleLoSpliceEvent`, `SetupCm`,
  `CMpeg::CbDemuxed`/`Update`, `AddLootEntryOid`); every v1 dispute is resolved.
- `table`: cross-checked against the class-descriptor and option tables.

**Runtime refutations worth remembering** (each overturned a confident static reading): S04 "guards alerted" (patrol), S05 "reaction to the bottle" (pathlink traversal),
JTMODE kind 1 = binocucom (it is recon; binocucom is kind 0), `g_pjt` = current player (always Sly), Bentley's bombs (dart + gadget object), the checkpoint manager
(transition request), `StopXfMotion` (it is `SlotUnsettle`), `AddSoVelocity` (it is `AddSoAcceleration`). Lesson: a structural reading of a single code path is not
a role; get a second signal before T1.

**Open human decisions** (master_names_v2 REPORT section 4): the two `FBounceLeaf` bodies; the two rich-text classes (`CRichText` vs the hud-TU class, proposed
`CRichText2`); `LoadClutsetFromBrx` vs `LoadSubglobBindingsFromBrx`; approval of the structural-T1 rule, the BHV class-word rule, `snd_CmdNN`, the 25 whole-game class words;
compiling arbitration's new manglings in the scratch build before committing.

## 5. Applying names: `tools/rename.py`

Status: working and tested in the scratch clone (`/home/claude/vclone`); **not applied to the real repo**.

```sh
tools/rename.py OLD NEW                    # OLD = current name or 0xADDR; NEW = plain or gcc 2.95 mangled name
tools/rename.py --csv batch.csv            # rows: addr,new_name[,new_mangled]
#   --dry-run [--diff]  --on-fail revert|stop  --log FILE  --commit-msg FILE  --jobs N  --allow-existing-uses  --allow-unverifiable
```
Needs the matchtool environment (`SLY2_M2C_INC` for m2c files). Exit 0 = all applied/already applied, 1 = something refused or reverted, 2 = stopped.

Policies:
- **Plain names when the signature is not confirmed**: an unmangled C-linkage identifier everywhere (`EvaluateBezierPos`), never a guessed or placeholder mangling. The linker
  symbol is a claim about the binary and matchtool cannot check references, so a wrong mangling would record something false. Upgrading later is another rename.
- **Confident mangled names** are used verbatim as identifiers (C files and `extern "C"` blocks in hand .cpp); promoting files to natural C++ is a separate cleanup.
- One rename changes, in one lexer pass per file: code identifiers, the name inside string literals (asm labels, inline asm), comments (to the plain name), the file name
  (`src/**/OLD.c` -> `<symbol>.c`), `symbol_addrs.txt`, `functions.csv` and `matched.csv`. It also sweeps the `func_XXXXXXXX` placeholder, which finishes upstream's partial
  renames (421 named symbols are still referenced as `func_` in 2,791 places). Data symbols get `// type:data` entries.
- **Safety**: name validation (C identifiers; a gcc 2.95 mangling grammar that accepts all 379 existing mangled names); collision checks (config, code tokens, case-insensitive
  file names, in-batch chains); **every touched file is rebuilt before and after and compared per address** (a rename can change codegen through gcse's symbol-hash order);
  on failure the offending renames are pulled out and retried one at a time. Idempotent and resumable (state is derived from the repo).
- Files that cannot be built here (51, mostly src/lib needing native ee-gcc 2.96 binutils) are refused unless `--allow-unverifiable`.

**Trial** (152 verifier-CORRECT rows): 148 applied (100 mangled + 48 plain) across 438 files, all verified MATCH; 3 refused (names already declared with C++ linkage in callers:
`ClsgClipEdgeToBsp`, `PruneBsp`, `BuildRotateVectorsMatrix`); 1 reverted automatically: `func_001A7518` -> `main` broke the match because gcc inserts an implicit `__main` call
into `main` (naming it is a source change). Independent checker `review_check.py`: 0 problems; clean re-run byte-identical; second run a no-op.

**Reviewing a bulk rename**: read the log counts and every non-applied reason; run `review_check.py REPO LOG [BASE]` (proves the diff is only the logged token swaps); skim
`git diff -M30% --stat`; then review the **names**, not the edits (plain-policy list, mangled signatures vs the Sly 2 body). One batch per commit, with the JSON log kept
next to it. Open questions for the maintainer are in `rename_tool/OPEN_QUESTIONS.txt` (plain-name policy, asm-label aliases, `main`, functions.csv sync, data symbols, CI on a
machine that builds src/lib).

## 6. Related: shared type headers

`reports/types_headers/MIGRATION.txt`: 12 generated headers (`include/sly2/{basic,lo,xfm,so,po,step,aseg,stepplayer,jt,players,cm,sw}.h`, 111 types) replace the private struct
copies in all 37 Lilly-port files (25,064 duplicated lines removed); 43/43 migrated functions still MATCH; 46 compile-time asserts check class sizes against the descriptor
table and 22 field offsets against runtime findings (a wrong value stops the build). Field offsets were measured by compiling probes with ee-gcc 2.95.3, not read by eye.
Conflicts (BASIC/CPLCY declared without a vptr in a few files, SM/SMA tail padding, Sly 1 ALARM/AMB text) are listed in `struct_conflicts.csv`. Not yet applied to the repo.
Names in headers and the rename tool should be applied in the same branch to keep declarations consistent.
