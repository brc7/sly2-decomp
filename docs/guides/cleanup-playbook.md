# Cleanup playbook: from a matching decomp to readable code

How the Sly 2 matching decomp went from m2c-shaped, per-function, offset-named code to named, documented, unified
source without losing the match, written so the next project (a Sly 3 decomp built the same way: g++ 2.95 for the PS2
EE, splat-style splits, a matchtool-style function checker) can do it in a fraction of the time. Each phase says what
to do, why it comes in that place, and what it cost us when we got it wrong. The agent-facing checklist is
[skills/decomp-cleanup/SKILL.md](../../skills/decomp-cleanup/SKILL.md); the details live in the guides linked from
each phase.

The order matters. Every later phase edits the files the earlier ones produced, and every phase is gated by the same
verification. The short version:

1. prerequisites: a matching build, a real full link, one verify command;
2. names and doc comments, subsystem by subsystem;
3. translation-unit merging (one source file per original file);
4. view unification, family by family, with a census to find the work and an island review at the end;
5. cross-family signature passes, done in lockstep;
6. mechanical sweeps, re-run rather than merged;
7. verification discipline throughout (the part that decides whether the rest can be trusted);
8. the g++ 2.95 traps that each cost a round;
9. integration: one branch, merges in order, checkpoints;
10. agent orchestration and quota;
11. snapshot-driven work: RAM snapshots of the running game for field typing, live classes and name checks;
12. 64-bit readiness: the exact 32-vs-64 danger test and its cheap fixes.

## 1. Prerequisites

Do not start cleaning until all three exist; each one later caught a class of error the others could not.

- **A matching build with a per-function checker.** tools/matchtool.py compiles one source file and compares every
  function and data symbol it defines against the original ELF, masking only the fields relocations fill in. This
  is the inner loop of every phase.
- **A full real link.** tools/link.py links the whole ELF from the objects, one section per object, and reports
  differing bytes against the original. A per-function checker cannot see data placement: the full link is what
  caught `.bss` objects moving when a declaration moved (section 8). See [full-link.md](full-link.md).
- **One verify command.** tools/verify.sh runs the whole-tree gates (progress from a deleted cache, matched.csv
  unchanged, doc links, full link) and prints one pass/fail summary. Every branch passes it before it is merged.
  A progress run must leave config/matched.csv byte-identical; make sure the tool writes it deterministically (line
  endings included) or every run is a false alarm.

Also settle the environment once (compiler bundle, wibo, the stub Python path, the m2c include directory) and write it
in one workspace note that every agent reads first; see [setup.md](setup.md).

## 2. Naming phase

Why first: names are what every later reviewer, agent and census reads. A view-unification agent that sees
`CanListenerHearNoise` and `listener->noiseType` decides correctly far more often than one that sees `func_` names and
`x4C8`. Names also touch every line, so doing them before structural work avoids two passes of conflicts.

Conventions (the full text is [naming-workflow.md](naming-workflow.md) section 1):

- locals, parameters and fields in descriptive `camelCase`; booleans as predicates (`isInWorld`, `hasTarget`);
- functions in `PascalCase` verb phrases; predicates start with Is, Has, Can or Should;
- no new Hungarian prefixes; existing cryptic Hungarian names (`FCheckLm`, `PloFind`) are renamed, clear ones kept;
- types keep their names in the first passes (renaming a type changes every mangled symbol that uses it);
- class placeholders in decimal, never mixed with hex;
- one prose doc comment above every function (what and why, not a paraphrase) and a header comment per file;
- `(inferred)` marks a meaning read from evidence rather than proven; `unknown4C8` only when nothing can be read.

Tools:

- tools/rename.py renames a linker symbol everywhere at once: config rows, every caller, C++ mangling (it keeps the
  signature part), asm-labelled virtuals. Never rename a function by hand: matchtool will not notice a caller still
  bound to the old symbol, only an `nm` comparison will.
- tools/fieldrename.py renames struct fields from a reviewed plan CSV, one header per batch, and lets the compiler's
  own errors find every use; it restores the tree if any object changes.
- tools/readability.py is the progress metric (documented share, placeholder names, Hungarian, cryptic locals,
  offset-named fields). Use it for trends and for picking the next files, not as an exact census.

How the naming went best: wave 1 split by subsystem (each agent renames what its directories define), then shared
header fields by one agent with fieldrename.py, then a second per-subsystem pass on what readability.py still flags,
then a C++-name sweep once rename.py handled mangling. Calibrate names: independent verifier rounds re-deriving
names from code agreed with the proposer most of the time but not always; keep the `(inferred)` marker honest. A swap
of two names (A becomes B, B becomes C) must be applied as a two-step map, never as search and replace.

## 3. Translation-unit merging

Per-function files from the decompiler become one source file per original file, so that file-scope state, inline
helpers and vtable emission behave as in the original. The full method and the mismatch table are in
[tu-merging.md](tu-merging.md); the lessons that generalize:

- Concatenate the per-function files verbatim first, then clean up; a hand-filtered draft loses things (a class
  `operator delete`, an unused local that sized the frame).
- A function moved into a larger file keeps its own tuning; callers that disagree about a callee's signature keep
  their own asm-labelled view of it until a later pass reconciles them.
- The static-initializer function and pool order are generated from the order of definitions; match them first.
- Inline asm functions can depend on the file name and line (section 8): merging changes both, so check every
  asm-heavy function and pin with `#line` where needed.

## 4. View unification

A *view* is a struct that re-declares part of a shared type for one function or file: a record padded up to the
fields it reads, a copy of a header type under a `V` name, a hand-made vtable-entry record called through by index.
Decompiled code is full of them (one per function that touched the type). Unifying them is the largest readability
gain after names and the largest source of risk. The method is in [tu-merging.md](tu-merging.md), "Views unified";
the per-round records are in [view-unification-history.md](../archive/view-unification-history.md).

**Families and ownership.** Partition the shared types into families (base object tree, player, AI, world and state,
camera, render, animation, sound/UI/scripting, gameplay objects). One agent per family per round, in its own
worktree; the family owns its headers and nobody else edits them in that round. An agent that needs a field in
another family's header uses a local derived struct or notes it for the owner. This rule is what made eight
parallel agents mergeable.

**Rounds.** Each round removes the views it can in a timebox, prioritized by number of copies, and lists what it left
and why. The next round starts from the integrated tree, so it sees the other families' header work. Expect three to
five rounds per large family; later rounds go after harder shapes (records padded over a base, hand vtable calls,
files without a header).

**The census.** Grepping for `V` suffixes finds the easy third. tools/cleanup/census/dump_offsets.py asks the real
compiler for the offset and size of every member of every header type; tools/cleanup/census/census_all.py then
classifies every struct defined in a source file by name, by layout (most named fields at a header type's offsets)
*and* by usage (the struct meets the type's name, its global, a pointer named after it or a function carrying it in
one statement), and tiers the results: views of named types, hand-vptr records, leading-pad views of unknown classes,
private vtable-entry copies, repeated helper records, genuinely local. The usage check is what made layout matching
precise enough to drive rounds. Re-run it after each integration and hand each family its tier list; sample the tiers
by hand before trusting counts.

**Representation rules** (decisions.md section 1 is authoritative):

- separate structs where views really conflict, never `void *` plus casts;
- derived structs for views that see more of a type (matched code keeps the base unchanged); a record padded over a
  base becomes a view derived from the largest base that fits in its padding;
- unions only where a kind or mode field selects the bytes, or where the original evidently had one representation
  (quadword rows moved as 128-bit values are union members, not casts);
- islands: small named forms that exist only to reproduce codegen, documented with the reason.

**Islands: timebox, then review by cause.** Inside a family round, an agent that cannot make a replacement match
tries the supported representations for about twenty minutes, then keeps the view renamed `<Type>Island` with a
one-line reason, and logs it with a cause class (register allocation, scheduling, address folding, type width or
signedness, aliasing, header clash, other). After the families finish, one island-review agent groups the log by
root cause and deep-dives each class without a per-island timebox. This split paid off: most islands shared a handful
of root causes (a bit-field declared with the wrong type, a constructor form, a missing union member, a header
declaration with the wrong return type), and fixing the cause removed the whole group. Only about a third were truly
matching-only. Raising the in-round timebox instead would have spent the same effort once per island.

## 5. Cross-family signature passes

Some fixes span families: a virtual a base declares and overriders in other families' headers implement (a render
entry point's arguments, a rotation that takes a matrix, a point-inside test, a widget's show), a constructor that
must become real, a slot whose return type is wrong. These cannot be done inside a family round without breaking
ownership, and done piecemeal they move virtual slots. Collect them in one list during the rounds and run one
signature agent after integration that changes each signature in lockstep across the base, every overrider and every
caller, verifying that no slot moves (relocations by symbol and `nm` catch it).

## 6. Mechanical sweeps

Some cleanups are the same edit in hundreds of places and need no judgement per site:

- private copies of the compiler's vtable-entry record replaced by the shared one (delete the copy, rename the type in
  its scope, rewrite the function-pointer field accesses as casts of the shared field);
- wrong-typed global externs (a global declared as a char array, an opaque handle or a scalar alias in some file)
  replaced by the header declaration, rewriting uses per the declaration that was in scope;
- repeated small helper records and inline helpers (clamps, vector combines, allocation requests) moved into shared
  headers under one modern name each, where folding keeps codegen; same-name helpers with different bodies are
  renamed, never merged.

Write each as a script over one file that prints what it did and skips listed lines, run it file by file with the
per-file check, and keep a list of the sites it must skip and why. **When the base moves, re-run the sweep on the new
base instead of merging its branch.** A sweep branch touches every file; merging it into a tree where families have
moved on produced hundreds of conflict hunks, each a chance to drop someone's rename. Re-running the script took
minutes and its skip list carried the judgement.

## 7. Verification discipline

The most important lesson: **matchtool's MATCH is necessary, not sufficient.** It masks every field a relocation
fills in, so a call that now resolves to another overload, another virtual slot or another assignment operator still
matches, and a linkonce copy that appears or vanishes is invisible. Every change is therefore checked three ways:

1. matchtool on every touched file and on every includer of a touched header (tools/cleanup/includers.py lists
   them; a wide header means the whole game directory);
2. object comparison against a baseline: tools/cleanup/objcmp.py compares the symbol set, `nm -S` sizes, section
   names and sizes, and relocations by target symbol. Relocations caught wrong overloads and namespace-mangled
   declarations; sizes caught a record that grew from 12 to 16 bytes; section names caught linkonce changes;
3. the whole-tree gates before a merge: progress from a deleted cache with matched.csv unchanged, the full link with
   0 differing bytes, tools/linkcheck.py, and tools/cleanup/docnames.py so docs never cite a name that no longer
   exists.

tools/cleanup/check.sh runs steps 1 and 2 for a list of touched files. Rules that made this reliable:

- **Build baselines once, before editing, in a separate worktree** at the base commit
  (`objcmp.py record --root <base worktree>`). A baseline built from the edited tree, or rebuilt after a merge,
  silently absorbs the regression. One baseline per integration base, shared by every check against it.
- **The full link catches `.bss` order.** Uninitialized objects are emitted in the order of their first declaration,
  including declarations in headers; moving or removing a declaration reorders `.bss` and every function still
  matches. Only the link shows it.
- **Summaries print the per-section table**, not just a total: "0 differing bytes" over the wrong set of sections, or
  a fallback that quietly took bytes from the original, looks identical in a one-line summary.
- **Agents report verification output, the coordinator re-runs it.** An agent's "all MATCH" covered only the files it
  thought it touched more than once; re-running on every includer before merging found the rest.

## 8. g++ 2.95 traps

Each of these cost at least one round. The full mismatch table is in [tu-merging.md](tu-merging.md) ("Mismatch causes
and fixes", "Traps") and the compiler facts in [MATCHING_NOTES.md](../MATCHING_NOTES.md).

- **Wrong overload, ambiguous `extern "C"`.** Converting a view to its base can select another overload of a callee;
  an `extern "C"` declaration inside a namespace identical to a file-scope one makes calls ambiguous once types agree
  (g++ 2.95 compares assembler names); a C declaration of a template member's mangled name clashes with the member.
  Delete the duplicate declarations; check relocations.
- **`.bss` order follows first declarations** (above): adding a global's declaration to a header can reorder objects.
- **File name and line hash into asm ordering.** The gcse ordering of inline-asm expressions depends on the byte sum
  of the presumed file name and the asm statement's line (a window of about 30 lines). Merges, renames and added
  comment lines can break asm-heavy functions far from the edit; `#line` pins hold them, and every edit re-checks the
  whole file. Renaming a file is not free for these functions.
- **Linkonce and key-function emission.** The first non-inline virtual defined in a file makes it the class's key TU
  (vtable plus out-of-line inline copies). Inline functions get a weak linkonce copy wherever they are not inlined
  (called before their body, address taken, explicitly instantiated). Watch new weak symbols and linkonce sections.
- **Silent compiler exit.** A local struct that redefines a header struct with virtual functions segfaults cc1plus;
  the driver exits with status 1 and no message. Making a header class polymorphic turns every same-named private
  view in its includers into this crash. With many errors the compiler can also print nothing.
- **Asm-labelled versus inline constructors.** g++ treats an asm-labelled constructor as returning `this`, which keeps
  the object live in a register; a constructor that wraps a C function must be written inline. An asm-labelled inline
  is not emitted from calls, which limits out-of-line copies to one object.
- **Member-template constructors in wide headers.** Any inline member added to a header included everywhere
  renumbers function-static labels in every includer (`nm` differs, code does not). A constructor only some includers
  need is written as a member template so it is compiled only where used.
- **Bit-field declared type sets extraction width.** A `ulong` bit-field is extracted with 64-bit shifts, a `uint`
  one with 32-bit shifts; comparisons can fold into mask tests. Pick the declared type the code's instructions show,
  and cast at odd uses.
- **Union member versus cast.** Reading a float's bits through a cast folds the offset differently from reading a
  union member; where the original read both ways, the type gets a union, not casts.
- **Named return value extension.** Inline helpers returning a struct used g++'s `return m { ... }` form to avoid an
  extra temporary; this is a g++ 2.95 extension that other compilers reject (a port needs overrides).
- **User-written destructors store the vptr.** Only an implicit destructor's body leaves the vptr store out; a real
  base destructor can therefore be impossible to write in matching source.
- **C library names in SDK C files become builtins.** Renaming a libm-style function to its real name (`fabsf`,
  `isnan`, `scalbn`, ...) lets gcc recognise it as a builtin and inline or fold it, so the call (or the whole
  function) changes. Never declare the library name directly: keep the old identifier and give it an `__asm__` label
  with the new symbol, at the declaration *and* at every call site that declares it locally. Only the platform-only
  builds (the Docker SDK build) compile these files, so run them before calling such a rename done.

## 9. Integration workflow

- **One integration branch per stage**, created from the last verified checkpoint; family branches merge into it one
  at a time, `--no-ff`, in a fixed order (base types first, then the families that depend on them).
- **Reconcile duplicates at merge time.** Two families often defined the same new type in different headers; keep one
  (the owner's), point the other's users at it, and verify. Resolve config CSV conflicts by keeping both sides' rows.
  tools/cleanup/take_theirs.py resolves hunks to the incoming side for files a branch owns; review the result.
- **Verify after each merge** with check.sh on the merged files and their includers; run the whole-tree gates at
  checkpoints, not after every merge (a full progress run is long on a small machine).
- **Checkpoints**: name each fully verified state (r7a, r7b, ...), stack them, and move them to the maintainer's
  machine as git bundles. Disable automatic gc on the receiving side while stacked bundles are in flight, since each
  bundle's prerequisites are the previous checkpoint's commits.
- **Finish parked work on its own branch first.** A branch stopped mid-round is resumed and verified on its own,
  then merged; merging half-done work into the integration branch made every later check ambiguous.
- Keep a short list of merge choices made (which side's field names won) for the maintainer's review.

## 10. Agent orchestration and quota

- **One worktree and one scratch directory per agent.** Agents share the repository's object store, so never use
  `git stash` (it is shared across worktrees, and `stash pop` restores files but not the index).
- **Assume restarts.** Server restarts killed every running agent several times. Agents commit after every file (or
  every unified view in a big file), never hold more than about twenty minutes of uncommitted work, and append to a
  progress log in their scratch directory after each commit. A resumed agent reads the log and `git status`, lets
  matchtool decide whether to finish or revert the half-done edit, and continues.
- **Timeboxes everywhere**: per round, per island, per experiment. A round ends with what it left and why; that list
  is the next round's input.
- **Keep the coordinator's log.** A queue file with status lines, decisions, running agents and what comes next
  survived restarts and quota stops and is where most of these lessons came from.
- **Quota.** When quota is short, run one or two agents, not eight: a stopped agent's branch must be resumed and
  re-verified, which costs more than it saved. Park branches at a clean commit and resume them one at a time.
- **The coordinator verifies.** Agents report; the coordinator re-runs the gates before anything is called done, and
  surfaces decisions the maintainer must make (a config alias, a representation choice) as soon as they appear.
- **Prompts**: a common prompt per phase (environment, rules, context to read, the task, how to verify, the final
  report format) plus one family line; the template is in the skill.
- **A second team (the port) coordinates through a message board outside both repositories**, not through either
  tree: one file per request, and one writer per log file (each side appends only to its own log), so no edit ever
  needs a merge. The port lives in its own repository and pins the decomp; requests cross the board, code does not.

## 11. Snapshot-driven work

RAM snapshots of the running game (PCSX2 save states; tools in tools/cleanup/snapshots/, rerun order in its
README) answer questions the code answers only slowly: which classes are alive in a level, and what a slot actually
holds.

- **Object discovery.** Every BASIC-derived object has its vtable pointer at +0xC (a few roots differ: animation
  curves at 0, actions at +0x18, particles further in). Scan the heap for words equal to a known vtable address,
  subtract the offset, and cross-check against the world's per-class lists, which also tells stale copies in freed
  memory from live objects.
- **Field typing across instances.** Classify each 4-byte slot over every instance of a class in every snapshot
  (zero, vptr, object pointer with the target's class, interior pointer, code, global, heap, small int, float) and
  merge with the header. This is what snapshots are good at: field types, pointer targets, and the live class set
  of a level.
- **What they are weak at:** the class behind a TU-private view. Prototypes and declared types only bound it
  ("is-a T", not "is T"), and layout matching against the inferred slots can propagate one wrong slot into many
  views. When a snapshot and the code disagree, the code wins: the code is what the game does; a snapshot is one
  moment of it.
- **Behaviour objects only exist while pushed.** Guard behaviours, mission and minigame states appear in RAM only
  while active, so "never seen" usually means "not captured in that situation", not dead code. States inside the
  situation matter more than more images of a level already seen.
- **Collecting states cheaply.** Warping by memory writes (the world's level-request block) through a PINE bridge
  that serves a job queue (one JSON file per job in an inbox, results in an outbox) captures one state per level
  unattended. After each load the game pauses on a memory-card dialog; dismiss it with SQUARE held through the
  joypad state (a forced-button mask does not reach menus). See tools/cleanup/snapshots/warp_tour.py.

**Name checking against snapshots** (tools/cleanup/snapshots/namecheck.py). Names and types imply kinds
(`p` prefix or pointer type: pointer; `f` or float: plausible float; counts and flags: small ints), checked against
every live instance of every class that contains the field. Paired-state checks compare fields across two snapshots
of a known situation (a coin collected, a jump, a guard alerted) and confirm or refute names like counters and
states. Mismatches that cluster at one layout region are usually sampling artifacts (few instances, a union, a slot
only used in some states), not offset drift: verify against the code that reads the field before moving offsets.

## 12. 64-bit readiness

The port compiles the decomp natively, so every TU-private view must put each field at the right 64-bit offset.

- **The exact danger test** (tools/cleanup/ptr64/dt.py, then judge.py). clang dumps each view's layout for x32 and
  x86-64; a view is dangerous only if a field the code actually accesses moves, judged against the real object's
  pointer slots (from the header, or the snapshots when the class is known). Use the EE data model: pointers are 4
  bytes but `long` is 8, so plain `long` is not a hazard. Fields never accessed do not count.
- **The cheap fix is deriving the view from the real base** (`struct V : REAL { ... }`), which carries the right
  pointer slots for free; next, type pointer slots that hide in pads (`void *` instead of a word of a byte array).
- **Write the judgement down.** When a view's pads are declared pointer-free (a file record, hardware words), record
  why in tools/cleanup/ptr64/evidence.json; judge.py reads it, and the next round does not re-litigate it.
