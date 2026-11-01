# Guide: running the asset extractors and exporters

All tools are Python 3 (stdlib; numpy only where noted) and live with the research reports under `reports/`. Run Python with `python3 -I` when it reads game
files. **Everything these tools produce from the disc is game data: keep it private** (never commit or publish extracted worlds, textures, meshes, audio
or script listings). You need your own disc image and the retail ELF `SCUS_973.16`.

Formats behind each tool: [world-file](../formats/world-file.md), [level-objects](../formats/level-objects.md), [textures](../formats/textures.md),
[meshes-scene](../formats/meshes-scene.md), [audio](../formats/audio.md), [splice-scripts](../formats/splice-scripts.md).

## 0. Overview

| Step | Tool (directory under `reports/`) | Input | Output |
|---|---|---|---|
| 1 | `assets/sly2_assets` (`cli`) | ISO + ELF | every disc file, world files LZSS-decompressed, `manifest.json` |
| 2 | `level_parser/sly2_assets` (`levelreport`) | ELF + `.brx` | per-object inventory, geometry offsets, coverage report |
| 3 | `whole_game/tools` | all 43 `.brx` | game-wide inventories, class hints, Splice catalogues, previews |
| 4 | `textures/sly2_textures.py` | `.brx` | PNG per bitmap x CLUT pair |
| 5 | `meshes/sly2_mesh.py` | `.brx` | OBJ/MTL (untextured) |
| 6 | `textured/sly2_export.py` | `.brx` | textured, placed glTF + OBJ of a level, or a character in bind pose |
| 7 | `audio/sly2_audio.py` | sound files | WAV (+ loop points, spectrograms) |
| 8 | `splice_scripts/tool/sly2splice.py` | `.brx` | decoded script listings, round-trip check |
| 9 | `vu_microcode/*.py`, `rendering/tools/*.py` | ELF / RAM dump | VU disassembly, render-list and VIF chain dumps |

Set once:
```sh
R=/home/claude/ref/reports                      # the reports tree
ELF=/path/to/SCUS_973.16
W=/path/to/out/worlds                           # decompressed world files from step 1
```

## 1. Extract the disc (`assets/`)

```sh
cd $R/assets
python3 -I -m sly2_assets.cli info    /path/Sly2.iso $ELF -v          # static records, disc CRC check
python3 -I -m sly2_assets.cli extract /path/Sly2.iso $ELF out/          # all worlds, sounds, movies, icon + out/manifest.json
python3 -I -m sly2_assets.cli decompress world_raw.bin world.brx        # one raw world
python3 -I -m sly2_assets.cli world world.brx --elf $ELF [--full]       # print the head
```
`extract` follows the ELF's 8 static records and every level table: 1,880 files (43 worlds, banks, ambience, per-language dialogue streams, music, 2 movies).
Note: this original package does not understand METHOD options and stops at head option 934 on 22 of the 43 files; use the level-parser copy (step 2) for anything past the head.

## 2. Parse every object (`level_parser/`)

```sh
cd $R/level_parser
python3 -I -m sly2_assets.levelreport $ELF OUTDIR $W/L02_nightclub_ep00_*.brx [more.brx ...] [--runtime $R/runtime/class_counts.csv]
python3 -I -m unittest discover -s tests          # 24 tests (sample tests need the ELF and the worlds)
```
Outputs: `coverage_report.txt` (byte attribution per category), `class_histogram.csv`, `class_coverage.csv`, `inventory_<level>.csv` (every object record: WE, depth,
cid, oid, isplice, offsets, status), `geometry_<level>.csv` (texture blob + every sub-glob offset/size), `runtime_compare_<level>.csv` (with `--runtime`).
API: `sly2_assets.level.parse_level(data, elf)` -> head, WEs, flat object list.

## 3. Whole-game runs (`whole_game/tools/`)

```sh
cd $R/whole_game                                  # writes into the current directory
python3 -I tools/run_levels.py . $W/*.brx         # coverage.csv + logs/failures.txt (expect 100.000% everywhere)
python3 -I tools/inventory.py . $W/*.brx          # objects_by_level.csv, objects_matrix.csv, we_names_by_level.csv, class_name_hints.csv
python3 -I tools/add_hints.py class_name_hints.csv
SLY2_ASSETS=$PWD/tools SLY2_REPORTS=$R python3 -I tools/splice_wholegame.py splice $W/*.brx   # splice/ catalogues + listings
python3 -I tools/media_check.py . $W/*.brx --previews L02_nightclub_ep00,L08_palace_ep00      # media_counts.csv + previews/
```

## 4. Textures (`textures/`)

```sh
python3 -I $R/whole_game/tools/sly2_textures.py LEVEL.brx OUTDIR [--raw] [--no-flip] [--we N] [--min-size N]
python3 -I $R/textures/make_contact_sheet.py OUT.png TILE COLS MAXN 'OUTDIR/*.png'
```
Use the `whole_game/tools` copy: it imports the level-parser package and works on all 43 files (the original in `textures/` fails on 22). File names encode
`we<WE>_shd<oid>_k<shdk>_f<frame>_p<pass>_b<bmp>_c<clut>_<w>x<h>_<T4|T8>.png`; `index.json` lists per-WE offsets. Defaults: rows flipped to top-down, RGB/A scaled x255/128
(`--raw` keeps stored values). Pass 0 is the colour texture.

## 5. Untextured meshes (`meshes/`)

```sh
python3 -I $R/meshes/sly2_mesh.py WORLD.brx OUTDIR [--we N] [--oid N] [--exclude-kinds 1] [--skip-hidden]
python3 -I $R/meshes/render_obj.py OUT.obj PREFIX --zup --views iso,top --crop 9000     # numpy preview renders
```
Each WE is written at its own origin (no proxy placement); use step 6 for an assembled scene.

## 6. Textured, placed scene or character (`textured/`)

```sh
cd $R/textured
python3 -I sly2_export.py                                        # Paris hub -> out/L02_paris_hub.{gltf,bin,obj,mtl} + out/textures/
python3 -I sly2_export.py --character 53 --name L02_sly --out out_sly          # Sly (JT class 53) in bind pose
#   options: --level N (load filter, default 2) --keep-markers --keep-carried --keep-fx --keep-hidden
python3 -I validate_gltf.py out/L02_paris_hub.gltf               # structural check
python3 -I make_previews.py                                      # preview_*.png / compare_*.png rendered from the glTF
python3 -I verify_runtime.py                                     # compare placed SOs with runtime/object_inventory.csv (S02)
```
Coordinates: glTF is Y-up metres (`(x, y, z) -> (x, z, -y) * 0.01`). Lit shaders are exported unlit; skinning, shader animation and the relight lighting are not exported.

## 7. Audio (`audio/`)

```sh
python3 -I $R/audio/sly2_audio.py export FILE [FILE ...] -o OUTDIR --spectro [--max-bank-samples N]
```
The format is detected from the magic (`SBlk`, `" KPV"`, `VAGp`). Banks export one WAV per waveform (lowest rate if several tones share it; `index.json` lists the sound
indices); VPK exports one WAV per channel group (stereo pairs joined) with a `smpl` loop chunk; VAGp a mono WAV. A small C ADPCM helper is compiled with `cc` if
available, otherwise pure Python is used.

## 8. Splice scripts (`splice_scripts/`)

```sh
cd $R/splice_scripts
python3 -I tool/sly2splice.py verify $W/*.brx                    # walk + byte-identical round trip of every entry
python3 -I tool/sly2splice.py dump $W/L02_nightclub_ep00_*.brx -o L02.splice.txt [--raw] [--width N] [--runtime-inventory $R/runtime/object_inventory.csv]
./regen.sh                                                       # all listings + OPTION_TABLE.tsv + tests (SLY2_WORLDS overrides the world dir)
python3 -I -m unittest discover -s tests                         # 10 tests
```
Environment: `SLY2_ASSETS` (head parser package) and `SLY2_ELF`. `--raw` prints numeric symbols/builtins/options for diffing.

## 9. Rendering and VU microcode tools

```sh
python3 -I $R/vu_microcode/listing.py [outdir]                   # prog0..prog6.txt: annotated VU listings (comments from comments.py)
python3 -I $R/vu_microcode/test_vudis.py                         # disassembler validation
python3 -I $R/vu_microcode/chains.py S04.eeMemory.bin KIND [count] [maxwords]    # VIF1 chains of the render entries of one kind
python3 -I $R/rendering/tools/rpl.py S04.eeMemory.bin            # render-list kind histogram (also vifwalk.py, gifsets.py, agg.py, subs.py, dis.py)
```
The RAM tools read PCSX2 save-state dumps (see [playtesting](playtesting.md#4-save-state-analysis-toolkit)).

## 10. Status of each tool

| Tool | Coverage | Tag |
|---|---|---|
| level parser | 100.000% of stream bytes, 121,153 objects, 43/43 files | [C:parse] |
| textures | 43/43 files, 71,774 pairs decoded, 0 errors | [C:parse] + [C:visual] |
| meshes | 11,244 SOs, 0 errors | [C:parse] + [C:visual] |
| scene export | Paris hub placement matches the S02 RAM state (173/174 roots within 2 units) | [C:runtime] |
| splice | 10,930/10,930 entries round-trip | [C:parse] |
| audio | one sample per kind decoded and checked | [C:decode] (others [H]) |
