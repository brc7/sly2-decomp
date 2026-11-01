# Guide: playtesting, patching and runtime analysis

How to go from "I edited a function in the decomp" to "I see it in PCSX2", how to give the PCSX2 debugger our names, how to read save states, and how
the planned autonomous experiment harness works. Tools live in `reports/patch_tools/`, `reports/runtime*/scripts/` and `reports/pine_harness/`.
Assumes PCSX2 2.x (paths below are macOS; Linux uses `~/.config/PCSX2`), your own BIOS, disc image and the retail ELF. Generated ELFs and save states are
game data: do not commit or publish them.

Tags: `[C:static]` = verified in this environment (byte comparisons, PCSX2 source reading); `[H]` = not yet run inside PCSX2 (no emulator here).

## 1. Install the patch tools

Copy `symelf.py`, `patchelf.py`, `pnach.py` and `sly2elf.py` into `<repo>/tools/` (or pass `--repo <repo>` / `SLY2_REPO=<repo>`). They need a repo checkout
where `python3 tools/matchtool.py check <file>` already works (patchelf compiles through matchtool with the matching compiler and flags). Optional:
llvm-objdump/objcopy/readelf for `--listing`, `--verify` and the tests (`brew install llvm`). Name database: `--names master_names.csv` or `SLY2_NAMES=...`.
All outputs are copies; the tools refuse to overwrite the input ELF. **The PCSX2 game CRC stays 07652DD9** for every output (a 4-byte `.pcsx2crc` section
compensates the XOR checksum when needed), so GameDB fixes, memory cards, save-state folders and pnach files keep applying `[C:static]`.

```sh
P2="$HOME/Library/Application Support/PCSX2"
```

## 2. Names in the PCSX2 debugger (symbol file)

```sh
python3 tools/symelf.py --sym ~/Games/PS2/Sly2.sym --t2                       # .sym next to the ISO with the same base name
python3 tools/symelf.py -o build/SCUS_973.16.sym.elf --t2 --verify             # or an ELF carrying a .symtab
```
The `.sym` is imported automatically (`<iso name>.sym`, format `%08x name[,%08x size]`) or added under Debugger -> Settings/Analysis -> extra symbol files; re-run
the analysis if names do not show. Names: T1 names, existing symbols, T2 drafts prefixed `T2_` with `--t2`, `func_XXXXXXXX` otherwise, junk labels and a few runtime-verified
globals (6,847 symbols, PT_LOADs byte-identical to retail) `[C:static]`.

## 3. Edit C, then test

### Workflow A: pnach (quick; works with save states)

1. Edit the function's source, e.g. `src/auto/func_001C67A0.c` (`AwardDprize91`, the coin award): `+ 1` -> `+ 10`.
2. Generate a patch into PCSX2's patches folder (PCSX2 loads every `SCUS-97316_07652DD9*.pnach` / `07652DD9*.pnach`):
   ```sh
   python3 tools/pnach.py src/auto/func_001C67A0.c --name "coins x10" -o "$P2/patches/SCUS-97316_07652DD9_coins.pnach"
   ```
   The pristine version comes from `git show HEAD:<file>` (`--orig` otherwise). Check the report ("in place, N word(s) differ").
3. In PCSX2: game Properties -> Patches, tick the patch (labelled patches must be enabled; `--unlabelled` writes auto-enabled lines). The default place=2 applies at
   boot **and every vsync**, so it survives a save-state load.
4. Iterate: edit, re-run step 2, reload patches (re-open properties or restart), load the state. Undo: untick or delete the file.

### Workflow B: patched ELF (big changes, many functions)

```sh
python3 tools/patchelf.py src/auto/func_001C67A0.c -o build/sly2_patched.elf --listing [--elf build/SCUS_973.16.sym.elf]
/Applications/PCSX2.app/Contents/MacOS/<binary> -elf "$PWD/build/sly2_patched.elf" -- ~/Games/PS2/Sly2.iso
```
(or drag the ELF onto the PCSX2 window while the disc is loaded). **Do not load a save state** to test an ELF patch: a state restores all RAM including the old code.

### How the splice works `[C:static]`

The edited file and its pristine version are compiled; the pristine functions that still match retail teach the absolute address of every relocation symbol and of the
file's .rodata/.data. Changed functions go in place if they fit (padded with nops), else into a code cave with `j cave` written over the original entry (jump tables
re-emitted). Caves: dead junk scraps (~2.2 KB) and, if needed, the top of RAM 0x1FF0000-0x2000000 (StartupMemMgr's `lui t1,0x200` patched to 0x1FF so the stack heap
starts below it; third PT_LOAD) `[H]` for boot safety. Writable static data always maps to its retail address. Unmodified files splice back byte-identically (150-file
identity test). `--manifest` lists every word; `pnach.py --diff RETAIL PATCHED` converts any patched ELF. Data-only tweaks need no compile:
`patch=2,EE,002BF738,word,43D70000` (jump apex 430.0f).

Demo patches (`patch_tools/demo/`): `coins_x10` (one instruction at 0x1C67C4), `higher_jump` (JtPlanLaunchVz uses the double-jump apex, retail **215.0**; v3 said 315.0), `jump_x2_cave`
(784-byte body in the RAM cave; boot fresh). In-game effect is `[H]` until tried.

## 4. Save-state analysis toolkit

A `.p2s` is a zip; `eeMemory.bin` is the 32 MB EE RAM (EE address A = file offset A). Set PCSX2's save-state compression to Deflate or store
(`SavestateCompressionType` 1/0): Python's zipfile cannot read the default Zstandard. File names: `sstates/SCUS-97316 (07652DD9).NN.p2s`.

| Script | Use |
|---|---|
| `runtime/scripts/slyram.py` | library + CLI: typed reads, class resolution (vptr at +0xC -> vtable slot 6 -> descriptor D_002BAAE8), per-class lists g_sw+0x3038 (12-byte SLs), oid hash g_sw+0x38, root DL g_sw+0x10, SO child DLs +0x64; `World.find_instances(cid)` for non-LO BASICs (CM, BHVs, policies) |
| `runtime/scripts/savevars.py` | decodes the save schema (g_sw+0x3A18) and prints every named `g_gs` variable per state |
| `runtime/scripts/probe.py` | one-shot summary: level, Sly, guards with BHV stacks and flags, camera stack, progression, save-var diff |
| `runtime/scripts/make_inventory.py`, `sgf_map.py` | object inventory and class counts for a directory of dumps; guard flag -> BHV map (ELF) |
| `runtime2/scripts/` (`jtstates.py`, `guards.py`, `statediff.py`) | Sly state table, guard stacks and modes, object-level diff between two states |
| `runtime3/scripts/` (`players.py`, `murraystates.py`) | current player, PO list, JTHURT/JTPLACES/JTMODE, Murray states |
| `runtime4/scripts/` (`gadgets.py`, `guardcombat.py`) | gadget table D_002BCD00 with names, guard KO/juggle, SO velocity (`common4.so_velocity`) |
| `runtime5a/scripts/` (`bentleystates.py`, `projectiles.py`, `objdiff.py`, `near.py`) | Bentley, bombs/darts, nearby objects |
| `runtime5b/scripts/` (`sys5b.py`, `clockdrift.py`, `tasks5b.py`) | system states: load in progress, pause menu, safehouse, clock slots, FILEREC XOR decode |
| `runtime6/scripts/` (`mission6.py`, `tasks6.py`, `relays6.py`, `widgets6.py`, `partner6.py`, `splice6.py`, `savediff6.py`) | jobs/tasks by save name, live RELAY/JSG positions, HUD counters, AI partner stacks, live Splice bindings |
| `runtime7a/scripts/` (`guards7.py`, `lasers7.py`, `lure7.py`, `world7.py`) | guard species/stacks/attacks, laser rigs, stimuli and the alarm-clock lure, runtime vs static class inventory |
| `runtime7b/scripts/` (`thiefnet7b.py`, `abilities7b.py`, `episodes7b.py`, `gadgets_confirmed7b.py`, `aseg7b.py`) | ThiefNet screen, gadget abilities and clock slots, episode menu and level table, ASEG event dumps |

```sh
python3 -I runtime/scripts/probe.py S02.eeMemory.bin S03.eeMemory.bin
python3 -I runtime/scripts/savevars.py S02.eeMemory.bin S03.eeMemory.bin
python3 -I runtime6/scripts/mission6.py --csv mission.csv S57 S58         # each round's README.txt lists its inputs
```
Runtime-verified globals: `g_sw` 0x3DD490, `g_cm` 0x2DDF40, `g_clock` 0x2DDED0, `g_game` 0x2DEB30, `g_pjt` 0x2E1E40 (always Sly; current player = g_sw+0x3EFC), `g_gs`
0x3D4A60, descriptors 0x2BAAE8, guard DL 0x3E1444, level index 0x3E1110, `g_cframe` 0x2BE538, `g_save` 0x2E65E0.

**Taking useful states.** Make pairs around one action (take A, do one thing, take B, diff). Objects re-allocate at identical addresses after a load, so offsets carry
across sessions of the same build. Pitfalls seen in rounds 1-7: info.txt labels were wrong several times (S04 is not alert; S14 is falling; S55 is the episode menu,
not a cutscene; S57 is a recon dialogue, not hands-off; the second "S72" is S73; there are no elephant guards), so trust RAM over notes; out-of-world pooled objects keep stale state; events (`BroadcastGameEvent`) leave no
trace in a snapshot.

## 5. The PINE experiment harness (designed, not yet run)

> v3: live PINE access now works through the bridge / pine_toolkit; see [live-pine](live-pine.md) for the workflow and the addresses confirmed in the running game, and [devtools](devtools.md) for the tools.

Goal: an agent runs "load state -> patch -> run N frames -> observe -> compare" with no human, on a headless Linux server or macOS (`pine_harness/harness_design.txt`).

- **PINE** (PCSX2's IPC; `[EmuCore] EnablePINE = true`, `PINESlot` 28011): Unix socket `$XDG_RUNTIME_DIR/pcsx2.sock` (macOS `$TMPDIR`); opcodes Read/Write 8-64, Version,
  SaveState/LoadState (numbered slots, asynchronous), Title, ID, UUID, GameVersion, Status. No input, screenshot, pause or step `[C:static]` (PCSX2 source).
- **Input and frame control** come from an EE patch (`SCUS-97316_07652DD9.pnach` + `input_stub.s`): FReadJoy's only call to `scePadRead` (0x1879E4) is redirected to a
  stub at 0x0F7000 that replays an RLE input script uploaded by PINE and can **park the EE at an exact input tick** ("hold"), giving deterministic input, frame-exact
  pause/step and consistent RAM snapshots. Prototype tested in a MIPS mini-interpreter only `[H]`.
- **Loop**: per worker one PCSX2 process (`xvfb-run pcsx2-qt -datapath D -nogui -fastboot -statefile S -- ISO`, OpenGL/Vulkan/software/null renderer), a "parked" slot-2
  state prepared once per base state (removes wall-clock jitter after LoadState), then per arm: load with a nonce protocol, apply PINE writes while parked, upload the
  input script, run to each observation tick, read fields (batched PINE reads) or save a state for screenshot + full RAM. Control and treatment arms, R repeats; a
  watchdog on `g_cframe` catches hangs.
- **Specs and verdicts**: the proposer writes `experiments/<id>/spec.json` (claim, base state, patch, input, observed fields, expectation with a predicted value) before
  seeing results; a separate verifier judges raw results -> `CONFIRMED(runtime, exp id)` / REFUTED / INCONCLUSIVE. Confirmed claims enter the docs as `[C:runtime exp-XXXX]`.
- Files: `pine_client.py` (stdlib client), `sly_input.py` (hook control), `run_experiment.py` (`run config.json spec.json`, `selftest`), `mock_pine_server.py` + `test_pine.py`
  (17 tests, all pass). The user supplies BIOS, ISO and states; the harness never downloads or copies a BIOS.
- Next steps: install PCSX2 on the server, `python3 -I run_experiment.py selftest config.json`, measure determinism and speed.

Experiments waiting for it: [open-questions.md](../open-questions.md) and rendering `EXPERIMENTS.txt` (E1 kind isolation, E2/E3 GS dump of shadow volumes, E4 relight
readback, E8 stereo).
