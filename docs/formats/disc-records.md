# Disc records: how Sly 2 finds files on disc

The retail disc has **no file table and no file names**. Every file is named by a 64-byte record (FILEREC) that some code or
data already holds: 8 static records in the executable, and records embedded in every world file.
Tags: `[C:static]` read from matched code; `[C:verified]` checked against real disc data (two sample worlds and the ELF); `[H]` hypothesis.
Source write-ups: disc_toc.md, assets/README.md.

## 1. FILEREC (64 bytes, 16 little-endian u32 `aw[0..15]`)

| Offset | Field | Meaning | Tag |
|---|---|---|---|
| 0x00 | aw0 | `lba = (aw0 ^ aw13) + 0xCA0` (sector; 0xCA0 = 3232, the data area starts right after IOPRP300.IMG) | [C:verified] |
| 0x04 | aw1 | `size = aw1 ^ aw15` bytes on disc (compressed size for LZSS files); 0 = no file | [C:verified] |
| 0x08 | aw2 | `fid = aw2 ^ aw14`, file identity, used only for equality tests | [C:verified] |
| 0x0C-0x33 | aw3..aw12 | never read by code; high-entropy, identical across duplicate records | [H] |
| 0x34 / 0x38 / 0x3C | aw13 / aw14 / aw15 | XOR keys for lba / fid / size | [C:verified] |

A record is empty when `aw1 == aw15`. Every caller converts a record to a "FLOC" with the same inline code:
```c
floc.kind = 2;                                    // 2 = sector range; kinds 0/1 were dev host/ISO paths (CdPath builds "cdrom0:\...")
floc.isector = (rec->aw[0] ^ rec->aw[13]) + 0xCA0;
floc.cb      =  rec->aw[1] ^ rec->aw[15];
return floc.cb != 0;
```
(`load_file_from_disc`, `Execute`, `ExecuteSwTransition`, `LoadWidgetSprites`, `PlayStreamsnd`, `PostSoundLoad`, ...) `[C:static]`.
Identity: `(a->aw[2]^a->aw[14]) == (b->aw[2]^b->aw[14])`; validity: `aw[1] != aw[15]` `[C:static]`.
No LCG/WAL-style encryption (that is a PS3 remaster construct) and no `sceCdSearchFile` call `[C:static]`.

## 2. The read path

| Function | Role | Tag |
|---|---|---|
| `sceCdRead` | the only sector reader | [C:static] |
| `ReadCdDirect(lsn, nsect, buf, mode)` | retry wrapper; adds no base offset | [C:static] |
| `CBinaryInputStream::OpenCdSectors` | CD-backed input stream: stores sector + size; Fill reads `(cb+0x7FF)>>11` sectors | [C:static] |
| `CBinaryAsyncStream::OpenSector`, `StartSpooling` | async stream for FMVs | [C:static] |
| `OpenStreamFromFileKey` / `OpenBasFromFileKey` | open by FLOC (kind 2 -> FOpenSector) | [C:static] |
| `UpdateCd` | disc check: CRC (reflected CRC-32, init 0xFFFFFFFF, no final XOR) of sector 1259331 (a sector number stored in the executable plus 0xCA0) against 0xACDD3EE4 | [C:static]; not run on a real ISO |
| `GetFileIdName` | retail stub returning a constant ("fid:#unknown") | [C:static] |

## 3. Static records in the ELF

These are ELF data addresses (the records are data of the retail executable, so the addresses are part of the format). ELF mapping: file offset = vaddr - 0x100000 + 0x80 (map through PT_LOAD to be safe). All 8 decode to plausible extents `[C:verified]`.

| vaddr | Used by | What | Decoded | Tag |
|---|---|---|---|---|
| 0x2A37E8 | `HandleGameEvent` (compared with the current world) | world | LBA 350144 = level 1 (intro, episode 1) | [C:verified] |
| 0x2A3828 | `StartGameFromSave` (world change) | world | same record bytes as 0x2A37E8 | [C:verified] |
| 0x2A3868 | the boot step queue (`BeginStartupSequence`) | boot/logo FMV | LBA 959782 | [C:verified] extent |
| 0x2A38A8 | boot step queue (warp oid 0x189) | **boot world** (title/menu), the crawl seed | LBA 335488 = level 0 | [C:verified] |
| 0x2A38E8 | `load_sly_icon_from_disc` | memory-card icon, raw | LBA 1169578, 25,688 B | [C:verified] extent |
| 0x2A3928 | `OnPauseItemReturnToHideout` (warp, flags 0x80) | world (quit to title?) | same as 0x2A37E8 | [C:verified] |
| 0x2A3968 / 0x2A39A8 | the movie player (`CMpeg::Execute`) | FMVs (intro/credits?) | LBA 959782 / 1388808 | [C:verified] extent, role [H] |
| 0x2A39E8 | `ResolveSwSaveVars` | `g_apchzWorld[9]` world-name pointers | intro, nightclub, palace, temple, prison, castle, train, sawmill, blimp | [C:verified] |

A further world record in `.data` (passed by the step queue) is all zero in the ELF: filled at runtime `[C:verified]`.

## 4. Records inside world files

Every world file carries records near the start of its decompressed stream (see [world-file.md](world-file.md)): the sound bank, ambience,
per-language streams, loose sound files, and the **level table**, which lists every level (44 entries, 43 valid; level 18 is empty) and is
identical in both sample worlds `[C:verified]`. So one world file gives every level. Other assets (sprite/texture files with magic `"ZRB."`
+ U16 version 6, in-level FMVs) are referenced from inside object data and are only reachable by parsing objects or carving gaps `[C:static]` (magic bytes from the ELF).

## 5. Extraction recipe (prototype tool exists)

1. Read the 8 static records at 0x2A37E8. 2. Decompress the boot world (0x2A38A8) with [LZSS](compression-lzss.md). 3. Parse the head up to
the level table; collect every record. 4. Repeat for each level (dedupe by fid or (lba, size)). 5. Carve unclaimed sector ranges: `00 00 01 BA`
= MPEG-PS movie, LZSS stream with `"ZRB."` v6 = sprite file, otherwise probably sound. 6. Name files by level/world/fid/LBA (no real names exist).
A stdlib Python prototype (`sly2_assets`: info / decompress / world / extract) implements this; validated on two samples, not yet on a full ISO `[C:verified]` (samples).

## Open questions
- `aw[3..12]` contents (noise? encoded dev names?).
- Run the disc CRC check (E2) and the full crawl on a real ISO; inspect the coverage gaps.
- Sound bank and stream formats (the IOP receives only the LBA).
