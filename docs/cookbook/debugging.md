# Recipe: debug a crash, a hang or a wrong behaviour

**Goal.** Go from a symptom (the game crashes, freezes, or does the wrong thing, in the original or in your mod) to
the function and the broken assumption, using the repository's tools and the systems guides.

**Systems.** Whichever the symptom belongs to; the "Invariants and gotchas" section of each guide in
[systems/](../README.md#systems-guides) lists the assumptions code most often breaks. Tools: `tools/matchtool.py`,
`tools/show.py`, `tools/link.py`, `tools/progress.py`, `tools/verify.sh`
([guides/setup.md](../guides/setup.md), [guides/full-link.md](../guides/full-link.md)).

## 1. Classify the problem

| Symptom | First question |
|---|---|
| crash or exception in the emulator | which function was running (the program counter), and what pointer was bad |
| freeze with the screen still drawing | which counter, state or wait never ends (hands-off, a busy behaviour, a relay command that never completes) |
| freeze with the screen frozen | is the main thread stuck in a level load (`ExecuteSwTransition`) or a loop |
| wrong behaviour, original game | is it a real game quirk (the code does it) or a misunderstanding of the code |
| wrong behaviour, your mod only | what you changed, and which original assumption it broke |
| a source edit no longer matches | a compiler question, not a game one: [MATCHING_NOTES.md](../MATCHING_NOTES.md) |

## 2. From an address to source

The emulator gives addresses; the repository speaks names.

1. **Address to function.** `config/functions.csv` lists every function with its start and end address; find the row
   whose range contains the program counter (or the return address in `$ra` for the caller). `config/symbol_addrs.txt`
   maps names to addresses for functions and named data, so a data address (a bad pointer's target) can be matched to
   the nearest global too.
2. **Function to file.** Search `src/` for the name. Many functions are defined under a readable C++ name with an asm
   label giving the symbol (`void UpdateJsg(JSGV *) __asm__("UpdateRelayScript")`), so search for the symbol in
   `__asm__("...")` strings as well as definitions. Every source file's header comment lists its entry points.
3. **The original code.** `python3 tools/matchtool.py target <function>` prints the original disassembly;
   `python3 tools/show.py <function>` prints it next to an m2c draft and the Sly 1 reference function when one is
   known. Because every function matches, the source **is** the original behaviour: read the C++ and trust it over any
   comment or doc that disagrees (and fix the comment).
4. **The system.** Find the function's subsystem in [architecture.md](../architecture.md) and read the guide's
   lifecycle section: knowing whether you are in load, post-load, update, render or teardown explains most crashes.

## 3. Common causes, by the invariant they break

| Cause | Typical symptom | Where the rule is |
|---|---|---|
| pointer kept across a level change (screens, globals, static caches pointing at objects) | crash right after a load | world-and-levels.md section 9; ui-and-hud.md section 8 |
| `LoadFromBrx` reading the wrong number of bytes | garbage or crash in the objects loaded after one class | object-model.md section 8 |
| oid reference used before post-load | null pointer in `Init` or `LoadFromBrx` | object-model.md section 4.3 |
| treating "in the world" as "live" | registration or update happening for inactive objects | object-model.md section 4.4 |
| state field written directly instead of through its setter | stale animation, attachment or camera tunnel | player.md section 10; ai-guards.md section 10 |
| unbalanced counters (hands-off, no-pause, letterbox, input lock, busy, music layer requests) | hero frozen, pause disabled, guard stuck in a mode, music stuck | world-and-levels.md section 7; ai-guards.md section 10; sound-and-music.md section 5 |
| pool exhausted (sounds, stimuli, slot heaps) | a creator returns null, then a crash in the caller | sound-and-music.md section 8; engine.md section 7.1 |
| option id used on the wrong class | script silently does nothing or touches an unrelated field | splice-scripting.md section 10 |
| synchronous script handler changing the world while native code iterates | crash or skipped objects in an update or message loop | splice-scripting.md section 10 |
| clock rate slot written without `SetClockRate` | time scale change has no effect | engine.md section 4 |

## 4. Look at the running game

- **Save states** reproduce a situation exactly; take one just before the problem.
- **Live memory over PINE** (PCSX2's remote memory interface; the project's PINE tooling is kept outside this
  repository): read globals and object fields while the game runs, or write them to test a hypothesis (the `[C:live]`
  facts in the docs were found this way). The world `g_sw` is the root of everything: its lists, the current job and
  task, the hands-off counters. Struct layouts come from `include/sly2/` (every header has compile-time size and
  offset asserts, so a header field's offset is trustworthy).
- **Save variables** are readable by name through the schema ([formats/save-variables.md](../formats/save-variables.md)):
  task states, the current character, coins.
- **The emulator's debugger** (PCSX2 has one with breakpoints and a disassembly view) is where a program counter and
  register values come from; set a breakpoint at a function's address from `config/functions.csv`.
- **No logging in the retail build.** The Splice `print` builtins (`BuiltinPrint` and its neighbours) were compiled
  out and return no value; `BuiltinGetDevFlag` returns 0. In a mod build, filling in `BuiltinPrint` with `printf` gives
  scripts a log (where the output appears depends on the emulator's console settings, inferred).

## 5. Your mod differs from the original

1. **Check what really changed.** `python3 tools/matchtool.py check <file.cpp>` lists the functions whose bytes no
   longer match in the files you edited; every other function is still the original. If a function you did not mean
   to change mismatches, an inline or header change reached it.
2. **Check the link.** `python3 tools/link.py` links the whole ELF and compares it with the original; its report and
   `build/link/mismatch.txt` give the owner of every differing byte and the reason. Changes that overlap the next
   object, or relocations to the wrong address, show up there.
3. **Bisect.** Revert half of the change and test again; with matching sources every reverted function is exactly the
   original, so bisection converges quickly.
4. **Compare with the original in lockstep.** Run the original and the mod from the same save state with the same input
   and compare the state you care about (positions, task states, counters) frame by frame over PINE; the port plan's
   differential-testing approach ([guides/port-plan.md](../guides/port-plan.md) section 10) is the same idea.

## 6. When the original game does it

If the matched code shows the behaviour is the original game's, it is a quirk, not a decomp bug. On the matching line,
document it (a comment at the function, a gotcha in the systems guide) and do not change the code. Fix it in a mod or
in the port, and say in the fix which original behaviour it replaces, so lockstep comparisons know to expect the
difference.

## Gotchas

- **Names can lie; code cannot.** Function and field names are hypotheses until marked otherwise
  ([engine.md section 1](../engine.md#1-how-knowledge-is-marked)); `(inferred)` names especially. The matched body is
  the ground truth.
- **Per-file views.** Many files read a struct through their own partial view (`BUTTONV`, `struct S` in a function's
  namespace). A field that looks different in two files may be the same offset under two names; compare offsets, not
  names.
- **Address-named globals** (`D_...`) are real globals that are not named yet; search `config/symbol_addrs.txt` and
  the source for other uses before concluding what one is.
- **Threads.** The render thread, the loading-screen thread, the memory-card worker and the movie thread run beside
  the main loop (engine.md section 4); a crash in one of them may come from data the main thread changed.

## How to test the fix

- Reproduce from the save state before and after the fix.
- For a mod: run `tools/matchtool.py check` on every touched file to confirm only the intended functions changed, and
  re-run the situations listed in the touched guide's gotchas.
- For a doc or comment fix on the matching line: `tools/verify.sh` must still pass (every function matching, no changed
  rows in `config/matched.csv`).
