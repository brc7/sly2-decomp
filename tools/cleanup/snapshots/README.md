# Snapshot tools

Tools that read PS2 RAM snapshots of the running game to type fields, find live classes and check names. Background
and lessons: [cleanup playbook, snapshot-driven work](../../../docs/guides/cleanup-playbook.md#11-snapshot-driven-work).

Game data never goes in git. Every tool takes the ELF, RAM images and save states as paths. The default work
directory is `build/snapshots/` (git-ignored). Shared options (`--root`, `--elf`, `--work`, `--census`,
`--census-config`, `--views`, `--vtjoin`, or the `SNAP_*` environment variables) are described in `snapcfg.py`. Every
script prints its usage with `--help`. Run them with `python3 -I`.

## Inputs

- The game ELF (`--elf`, default `disc/SCUS_973.16`).
- RAM images: `WORK/ram/*.bin` (32 MB `eeMemory.bin` dumps). `savestate.py extract STATE.p2s WORK/ram eeMemory.bin`
  takes them from PCSX2 save states. Images are deduplicated by md5 (cached in `WORK/ram_md5.txt`).
- `offsets.json` from `tools/cleanup/census/dump_offsets.py` (`--census`, default `build/census`). `census_tiers.json`
  from `census_all.py` is optional.
- For view resolution only: a view-plan JSON (`--views`) listing the TU-private views, with `file`, `line`, `view`,
  `fix`, `real_type`, `type_chain`, `candidate_type` and `milestone`.

## Rerun order

1. `vtjoin.py` (needs `--views`): gives the class of a view that is `this` of a virtual function. Writes `vtjoin.json`.
2. `snapshot_objects.py`: finds objects (vptr at +0xC under BASIC) and cross-checks them against SW's per-class
   lists. Writes `objects/*.npy` and `classes.json`.
3. `infer_layout.py`: types every slot across the instances and merges the result with the headers. Writes
   `layouts.json`.
4. `resolve_views.py` (needs `--views`): gives the class behind each view. Writes `view_resolution.json`.
5. `report.py`: writes `REPORT.md`.
6. `namecheck.py [extract|check|pairs|all] [--spec F]`: checks names and types against the snapshots. Writes
   `namecheck/namecheck.json`.

The whole chain takes under a minute for about a hundred images.

## Collecting states

`warp_tour.py` captures one save state per level. It warps by memory writes through a PINE bridge job queue and
dismisses the memory-card dialog with SQUARE. It needs the external PINE toolkit (see its docstring).

## Related

- `../ptr64/dt.py` + `../ptr64/judge.py`: the exact 32-vs-64-bit danger test. `judge.py` reads `classes.json`,
  `layouts.json` and `view_resolution.json` from the work directory. Hand judgements go in `../ptr64/evidence.json`.
