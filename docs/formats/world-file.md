# World file (BRX) format, Sly 2 PS2 (SCUS-97316)

A world (level) file is one LZSS-compressed stream ([compression-lzss.md](compression-lzss.md)) located by a FILEREC
([disc-records.md](disc-records.md)) and read front to back by `LoadSwFromBrx` ([world-sw.md](../engine.md#loading-a-level)).
Merged from the asset-extraction write-up and the brx/sound/save TU write-ups. Status tags in this page:
**confirmed** = `[C:verified]` (read from matched code and checked against both sample worlds); **code** = `[C:static]` (read from matched
code, not exercised by the samples); **hypothesis** = `[H]`.

**Key correction to the original draft:** the "world entries" are **proxy sources**. Each entry's object list is instanced `c8` times at load
(the original plus `c8-1` `CloneLoWithParent` clones); the copies form the pool that proxies and spawners draw from (`LoadProxyFromBrx`,
`AcquireProxySpare`). The index of the entry being loaded (`SW::isrcLoading`) is the `isrc` in `proxyId = (isrc<<16)|icopy` `[H r2 brx.md, r5 proxy.md]`.
The live Paris hub has 287 entries; entry 0 is `f_nightclub_exterior_vz` with 2 copies `[C:runtime]`.

Status tags:
* **confirmed**: read straight from matched decomp code AND checked against both sample worlds.
* **code**: read from decomp code, but the samples don't exercise it (for example the count is 0 in both).
* **hypothesis**: inferred from the data or by analogy. Treat it as unverified.

Samples used:
* `boot_world` = record 0x2A38A8 (LBA 335488, 9,969,830 bytes compressed, 17,420,982 decompressed);
* `london` = record 0x2A37E8 (LBA 350144, 1,949,759 bytes compressed, 4,084,599 decompressed).

The "london" text at the start of `london` is really `abandon_icon` (see section 3). That file is level 1 in the level table: world 0 ("intro"), episode 1.
**v2 note:** `london` = L01 is the **front-end/menu world** (only 3 WEs: `abandon_icon`, `swf_ui`, `dvd_menu`; the episode menu WIDGET131 runs here); `boot_world` = L00 is the playable prologue (`jb_intro_*`) `[C:parse whole_game]` `[C:runtime S55]`. The level parser now reads **every** world file to the last byte (43 files, [level-objects.md](level-objects.md)); all `[C:verified]` tags of this page are covered by that `[C:parse]` result.

## 1. Container

| Property | Value | Status / source |
|---|---|---|
| Location | One extent per file: `lba = (aw0^aw13)+0xCA0`, `size = aw1^aw15` (section 2) | confirmed: the world loader `ExecuteSwTransition` builds the FLOC this way |
| Compression | The whole file is LZSS, with no header and no stored length | confirmed: `SetDecompress` is called before `LoadSwFromBrx` |
| LZSS | Flag byte, LSB first, bit=1 means a literal. A reference is u16 `v`: ring index `v & 0x1FFF` (absolute index in an 8 KB ring; write cursor = output position mod 0x2000), length `(v>>13)+3` | confirmed: `Decompress` (`src/game/core/binstream/bis.cpp`). Byte-identical to SlyTools `sly_decompress.hpp`. Both samples decode to clean, fully parseable data |
| Initial ring | On PS2 the ring is the scratchpad (`0x70000000`), which is not cleared. Tools zero-fill it | code. Both samples parse fine with zero-fill, but no check proves that no early reference points into unwritten ring slots |
| Endianness | Little-endian throughout | confirmed (`bis.cpp` U16Read/U32Read/S32Read) |
| Align(n) | Aligns the **absolute offset in the decompressed stream**. The read pointer walks a 0x2000-byte window that starts at an aligned address, and pending bytes are moved to the window start on every refill | code (`bis.cpp` Align + Decompress); confirmed at the head/object boundary (section 9) |

Reader primitives (`include/bis.h`, `src/game/core/binstream/bis.cpp`):

| Name | Address | Reads |
|---|---|---|
| U8Read / S8Read | `U8Read` / `S8Read` | 1 byte |
| U16Read / S16Read | `U16Read` / `S16Read` | 2 bytes |
| U32Read / S32Read / F32Read | `U32Read` / `S32Read` / `F32Read` | 4 bytes |
| Read(cb) / Skip(cb) | `Read` / `Skip` | cb bytes. A **negative cb reads nothing** (CbRead's loop doesn't run) |
| file record | inline Read(0x40) (`CBinaryInputStream_ReadCb64`) | 64 bytes |
| ReadVector4 | `ReadVector4` | 16 bytes |
| string16 | `CBinaryInputStream_ReadStringSw` | `U16 cch; cch bytes`. No NUL on disc. cch=0 means a NULL pointer |
| skip string16 | `CBinaryInputStream_SkipStringSw` | `U16 cch; skip cch` |

## 2. FILEREC (64 bytes)

| Offset | Type | Field | Status |
|---|---|---|---|
| 0x00 | u32 | aw0. `lba = (aw0 ^ aw13) + 0xCA0` | confirmed: every record in the samples decodes to a sector >= 3232 |
| 0x04 | u32 | aw1. `size = aw1 ^ aw15` (bytes on disc; 0 = none) | confirmed |
| 0x08 | u32 | aw2. `fid = aw2 ^ aw14` | confirmed. Decoded fids are small integers: worlds 0x03..0x88, streams 0xEC..0x7A7, movies 0x808/0x80A |
| 0x0C..0x33 | 10 x u32 | aw3..aw12 | hypothesis: never read by code. They look random. They are identical across duplicate records of the same file (ELF 0x2A37E8, 0x2A3828 and 0x2A3928 are byte-identical) |
| 0x34 | u32 | aw13 = lba key | confirmed |
| 0x38 | u32 | aw14 = fid key | confirmed |
| 0x3C | u32 | aw15 = size key | confirmed |

A record is empty when `aw1 == aw15` (size 0). Level 18 of the level table is empty.

## 3. Head layout (`LoadSwFromBrx`)

Parse order. All offsets are in the decompressed stream.

| # | Section | Reader function | london | boot_world |
|---|---|---|---|---|
| 1 | world-entry table | `LoadSwFromBrx` | 0x0000..0x00FE | 0x0000..0x4E6E |
| 2 | sound section | `LoadSoundSectionFromBrx` | 0x00FE..0x0660 | 0x4E6E..0xAA94 |
| 3 | save variables | `LoadSaveSchemaFromBrx` | 0x0660..0x132D5 | 0xAA94..0x1D709 |
| 4 | level table | `LoadLevelTableFromBrx` | 0x132D5..0x13FED | 0x1D709..0x1E421 |
| 5 | localized strings | `LoadSwLocalizedStringsFromBrx` | 0x13FED..0x17157 | 0x1E421..0x22628 |
| 6 | plane tree | `LoadSwCellsFromBrx` | 0x17157..0x17159 (0 nodes) | 0x22628..0x22936 (39 nodes) |
| 7 | options of `g_sw` | `LoadOptionsFromBrx` | 0x17159..0x1717F | 0x22936..0x22956 |
| 8 | options of `g_cm` | `LoadOptionsFromBrx` | 0x1717F..0x1718D | 0x22956..0x2297C |
| 9 | Align(0x10) | `Align` | -> **0x17190** | -> **0x22980** |
| 10 | per world entry: WE data + objects | loop in `LoadSwFromBrx` | 0x17190..EOF | 0x22980..EOF |

All of sections 1..9 parse with no leftover bytes and no errors in both samples **[confirmed]**.

### 3.1 World-entry table

```
S16 cwe
cwe x {
  0x00  byte[0x40] block   (the entry's name block)
  0x40  S32 x5C
  0x44  S32 x60
  0x48  S32 x10            (proxy-source id)
  0x4C  S32 x14
  0x50  S32 c8             (number of copies)
}                          (0x54 bytes per entry; no alignment)
```

| Field | Meaning | Status |
|---|---|---|
| block | **Not a disc FILEREC.** In both samples it is ASCII `"FK$X"` followed by a NUL-padded name of up to 60 chars, e.g. `abandon_icon`, `swf_ui`, `dvd_menu`, `jb_intro_vz`, `explode_whack_flash_small`. It is passed only to `GetFileIdName` (`GetFileIdName`, a retail stub) for the load-spinner label. Probably the dev build's name-form FILEREC | confirmed (data + code) |
| x5C | If nonzero: `psw->iwe = -1` and the objects are not activated (`cC++`) instead of calling vt[9] (Remove) on each. In `boot_world`, every `jb_intro_*` entry and `jb_shadows` have x5C=1 | code; the meaning ("deferred/spliced scene") is a hypothesis |
| x60 | Stored only. It is 1 for `abandon_icon` | hypothesis: flag |
| x10 | Passed to `LoadSwSpliceFromBrx` as the id of the WE's script/ref table. It equals the **oid of an object in that WE's object list** (e.g. `explode_whack_flash_small` x10=3901, and the objects with cid 6 have oid 3901) | hypothesis (strong): root object oid |
| x14 | Stored only. Mostly increasing (152701, 152702, ...) | hypothesis: second id |
| c8 | Instance (copy) count of this proxy source. The object list is cloned `c8-1` times (`CloneLoWithParent`) into `WE.apo` (`c0*c8` slots). It also sizes `pv18` (c8 x S32, filled with -1) | confirmed (code); values 1..41 |

Counts: `london` cwe=3, `boot_world` cwe=239.

### 3.2 Sound section (`LoadSoundSectionFromBrx`)

```
FILEREC bank                         sound bank -> IOP (snd_BankLoadByLoc(lba)); kept if fid unchanged
S16 csnd ; csnd x S8 kind            anKind[]
S16 csfx                             size of the aisnd[] map
csnd x S16 sfxid                     aisnd[sfxid] = j   (count is csnd, NOT csfx)
S16 camb ; if camb > 0: FILEREC ambience
camb x S16 ambid                     (max 6 slots in SNDMGR)
U8 clang ; clang x U8 lang           the entry matching the console language is chosen
S32 cstream
cstream x {
  S16 id ; S16 n48
  clang x { FILEREC rec ; S8 n4C ; F32 g50 }
  S32 cpair ; cpair x 8 bytes
}
S32 cfile ; cfile x { S16 id ; FILEREC rec }
```

| Field | london | boot_world | Notes |
|---|---|---|---|
| bank | lba 351104, 596,968 B | lba 340384, 1,206,504 B | confirmed |
| csnd / csfx | 180 / 682 | 295 / 690 | confirmed |
| ambience | none (camb=0) | lba 341088, 6,588,416 B; ids 65,66,67 | confirmed |
| clang / langs | 1 / [0] | 1 / [0] | NTSC-U disc: one language |
| cstream | 2 | 61 (lba 1205954..1207693, 3.5 MB) | confirmed |
| stream.g50 | 4.23, 3.958 | 1.8 .. 21.0 | **duration in seconds** `[C:parse audio_browser]` (v3; was hypothesis): 7,849 / 7,970 stream records within 0.05 s of the duration predicted from the FILEREC size at 22.05 kHz mono PS-ADPCM, median difference 0; decoded stream1259 = 4.886 s = its g50 |
| stream.n48 | 55 | 53/54/55/60/63 | unknown |
| stream.n4C | 0 / -1 | -1, 0, 2 | unknown (S8) |
| stream.pairs | 30, 29 | 15..119 | hypothesis: lip-sync or subtitle keys. The count grows with g50 (about 7 per second). 8 bytes each, layout unknown |
| cfile | 2 (ids 34, 35; 2,254,848 B each) | 1 (id 36; 11,696,128 B) | confirmed (records); content type unknown |

### 3.3 Save variables (`LoadSaveSchemaFromBrx`)

```
U16 n ; n x { U16 value ; string16 name }   -> the world's save-variable symbol table (`SW::savevars`)
U16 c ; c x 10 bytes                        -> blob, then ResetGsIfSchemaChanged
```

Both samples have n=960 symbols and c=5461 trie entries, identical in both files. The 10-byte records are the save-variable trie `{s16 sym; s8 kind; u8 cbits; u16 ib; u16 ientChild; u16 ientSibling}` (sly2/sw.h SVENT) mapping names to offsets in the save blob `[C:runtime]` (decoded in RAM: 4,472 variables; see [save-memcard](../engine.md#713-save-data-and-memory-card-worldstatesavecpp-uimemcardcpp) and [save-variables](save-variables.md)). The U16 per symbol is its symbol id.

### 3.4 Level table (`LoadLevelTableFromBrx`)

```
U32 clevel
U32 nUnused                (= 30 in both samples)
clevel x {
  FILEREC rec
  U8 world
  U8 episode
  U8 fEntry                if != 0: this record becomes the world's entry (hub) record
  U8 n ; n x U16           the level's filter list
}
```

| Fact | Status |
|---|---|
| Both samples carry the **same** 44-level table. 43 records are valid. Level 18 (world 5 "castle", episode 7) has size 0 | confirmed |
| Level 0 = boot world (LBA 335488). Level 1 = `london`/0x2A37E8 (LBA 350144). The ELF static records point into this table | confirmed |
| Exactly 9 levels have fEntry=1, one per world 0..8. Each is that world's episode-0 level (hub) | confirmed |
| world is 0..8 and indexes `g_apchzWorld` (intro, nightclub, palace, temple, prison, castle, train, sawmill, blimp) | confirmed (ELF strings) |
| episode is 0..29. `nUnused` = 30 = the number of distinct episode values | hypothesis |
| Episode 7 appears in worlds 2, 3, 4, 5 and 8; episode 4 in worlds 1 and 5; episode 26 in worlds 7 and 8. So "episode" is a level-kind id shared across worlds, not a story episode | hypothesis |
| `list` always has n=4, shaped `[64, W, 67, L]`. W depends only on the world (65, 74, 80, 84, 85, 87, 95, 100, 106). L is 68 for every hub and otherwise depends only on the episode value. Read via `GetLevelListEntry(i)`; its use is not traced | hypothesis: dependency/package ids |
| The 43 valid level sizes sum to 284,382,717 bytes (compressed) | confirmed |

The full table is in the appendix below.

### 3.5 Localized strings (`LoadSwLocalizedStringsFromBrx`)

```
U8 clang ; clang x U8 lang
S16 c ; c x { S16 id ; clang x string16 }   only the g_nLang string is kept; the others are skipped (CBinaryInputStream_SkipStringSw)
```

`london` has 359 entries: the copyright line, menu text ("New Game...", "Back"), episode titles (ids 109..128: "A Shadow From The Past", ...), loot names, gadget descriptions. Ids are not sorted. **[confirmed]**

### 3.6 Plane tree (`LoadSwCellsFromBrx`)

```
U16 c ; c x { VECTOR4 plane (16 bytes) ; S16 front ; S16 back }
```

A child index >= 0 links to another node. A child of -1 assigns a leaf number `p->n++` (`p->n` leaves total).
In `boot_world` the planes are axis-aligned normals (1,0,0)/(0,1,0)... with a distance in w. **Hypothesis: a BSP of world regions.** `london` has 0 nodes. **[layout confirmed]**

### 3.7 Options of g_sw and g_cm (`LoadOptionsFromBrx` -> `LoadOptionFromBrx`)

```
repeat { S16 eopid ; if eopid < 0: break ; value by g_aeopid[eopid].otyp }
```

Each option id indexes the EOPID table `g_aeopid` (`include/sly2/eopid.h`: the value type `otyp`, the flags, the file word); the loader masks the flags with `& 0x2580`.

Value sizes by otyp:

| otyp | Bytes |
|---|---|
| 0 | 1 |
| 1, 5, 6, 13 (RGBA), 0x1xxx, 0x2xxx (oid) | 4 |
| 7, 14..17, 19..21, 23 | 2 |
| 10 | 8 |
| 2, 4, 9, 11 | 12 |
| 3, 12 | 16 |
| 22 | U16 c + 2c |
| other | 0 |

A grf with 0x400 set means a method option. Its arguments are U16 otyp (bit 0x8000 = U8 count list) + a `ReadOptionArgFromBrx` value. The original head parser did not implement them and stopped on 22 of 43 files (head option 934 on g_sw, 44 instances game-wide); the level parser does `[C:parse]`; layout in [level-objects.md](level-objects.md#4-common-building-blocks-cparse).

Samples:
* `london`: g_sw = {913: 1, 923: RGBA 0x80000000, 914: (230,260,250), 915: 0.0, 916: 95.0}; g_cm = {164: 10.0, 165: 1000.0}.
* `boot_world`: g_sw = {913, 914, 915, 916}; g_cm = {164..169}.

**[confirmed: both lists end exactly where Align(0x10) lands on the next structure]**

## 3.8 The loaded entry

Each entry becomes a proxy source (`PROXYSRC`, the world's `SW::aproxysrc` array): its objects, copy count, deferred count, ids and
name block. The in-memory layout is the header's ([level-objects.md](level-objects.md) section 2 describes what the fields mean).

## 4. Per-world-entry data (after the head)

> Superseded in detail by [level-objects.md](level-objects.md) (all sub-structures decoded and parse-verified, every class payload) and [textures.md](textures.md) (the `LoadShadersFromBrx` tables). The text below is the original first walk, kept for its sample offsets.

For each WE i in order (loop body of `LoadSwFromBrx`):

```
S32 cbBlob ; S32 idBlob            LoadSwSharedHeaderFromBrx; "reload" = idBlob != previous idBlob
S32 fExtra                         (adds 4 x string16 / extra fields in the ref reader)
U32 cref ; cref x Ref              LoadSwSpliceFromBrx (Ref = DeserializePair: U8 tag|0x80 next, payload by tag; 0xFF = null)
                                   (if fExtra: 3 x string16 after each Ref)
<LoadShadersFromBrx>                    U16 n x LoadClutFromBrx ; U16 n x LoadBmpFromBrx ; U16 n x LoadSaaFromBrx ;
                                   U16 n x LoadShdFromBrx (0x2C-byte structs) ;
                                   LoadSwSharedDataFromBrx: Align(0x10) ; cbBlob bytes (uploaded only if reload, else skipped)
U32 n ; if n: U32 c8 ; U32 c10 ; n x {U16,U16,U16} ; c8 x VECTOR4 ; c10 x S32        LoadSwZoneBspFromBrx/LoadZoneBspFromBrx
--- object list (LoadSwObjectsFromBrx, LoadSwObjectsFromBrx) ---
S16 cobj
cobj x {
  S8  k                            k>0: k x S16 filter list follows; k=0: always create;
                                   k=-1/-2: create unless a save-state check (0x805/0x405) says it is already gone;
                                   k<-2: Read(k*2) is negative, so nothing is read; the filter uses a stale list
  [k x S16]
  S16 cid ; S32 oid ; S16 n(isplice) ; S32 cb
  cb bytes                         the object's own data (vt[14] LoadFromBrx). Skippable by cb
}
Align(0x10)
```

Per-object record fields (`LoadSwObjectsFromBrx`, matched) `[C:static]`:

| Field | Type | Meaning |
|---|---|---|
| k | S8 | filter kind: `k>0` -> `k` S16 filter ids follow, object created only if `IsCurrentLevelInList` matches the current level index (`GetCurrentLevelIndex`); `k=0` always; `k=-1/-2` create unless the save state (query 0x805/0x405 on `(oid, MakeProxyId(iwe))`) says it is gone; `k<-2` reads nothing (stale list) |
| filter ids | k x S16 | level/job filter list |
| cid | S16 | class id (index into the class descriptor table); PROXY-derived classes go through `LoadProxyFromBrx` |
| oid | **S32** | object id (Sly 1: S16) |
| isplice | S16 | Splice frame index (-1 = none); resolved by `GetSpliceFrame` |
| cb | **S32** | byte size of the payload (new in Sly 2): filtered objects are skipped with `Skip(cb)` |
| payload | cb bytes | consumed by the class's vt14 `LoadFromBrx` (LO base: an options loop `S16 eopid ...`; SO: XF table, GLOBSET, parts, mass) |

JOB/TASK objects (CID 0x84/0x85) are created even when filtered, with +0x50 = 1 `[C:static]`.

Status:
* The object-record layout is **code** (`LoadSwObjectsFromBrx`).
* The `cb` size field is **confirmed by walking**: in `london`, WE 0 has 31 objects at 0x179C4. Walking them by cb ends at 0x76E21. Align(0x10) gives 0x76E30, which is exactly where WE 1's header starts (`cbBlob=0x8800, idBlob=0x411C7D26, fExtra=0, cref=1`).
* In WE 0 of `london`, the header fields line up: cbBlob=48, the 48-byte blob sits at 0x17990 (16-aligned), and `U32 n=0` at 0x179C0 sits right before `S16 cobj=31` at 0x179C4.
* The sub-structures of `LoadShadersFromBrx` are now decoded: [textures.md](textures.md).

Observations:
* `idBlob` values look like 0x411Cxxxx (0x411C70BF, 0x411C7D26, 0x411C7D54); the blob is re-read only when idBlob changes. The blob is the WE's texture image data (bitmaps + CLUT colours) `[C:parse]` ([textures.md](textures.md)).
* Class ids in the object lists are 0x03 (XFM), 0x05, 0x06 (PROXY), 0x0E, 0x11 (PATHZONE), 0x1A (EXIT), 0x1F (EXPLG), 0x21 (EXPLS), 0x30 (SO), 0x6D (ASEG), 0x7E (WIDGET), 0x82 (WIDGET subclass) and so on (names: [`include/sly2/classes.h`](../../include/sly2/classes.h)). Most objects have `n = -1` (no splice frame). cid 0x84/0x85 objects are always loaded even when filtered. See [`include/sly2/classes.h`](../../include/sly2/classes.h) for the mapping.

### Relation to Sly 1 (ProjectCane `brx.cpp`)

| Sly 1 | Sly 2 |
|---|---|
| `LoadSwObjectsFromBrx`: `U16 count`, then per object `S16 cid, S16 oid, S16 isplice` and `pfnLoadLoFromBrx`. No size field, so a parser must understand every class | `S16 count`. Per object: an extra `S8 k` (+ filter list), a **32-bit oid**, and a **32-bit byte size `cb`** before the payload. The game skips filtered objects by cb, so a tool can walk the whole object tree without understanding any class: a big improvement for extraction |
| Options use `g_aeopid`. A method option is grf 0x400 + S16 otyp args | Same scheme. otyp is U16 with a 0x8000 list bit, and grf is masked by the caller (0x2580 file / 0x2031 copies) |
| One world = one object tree | A world = N world entries (name + root oid + instance count), each with its own texture blob and object list. `boot_world` has 239 (props, effects, cut scenes) |

## 5. Other files referenced

* Sprite/texture files (`LoadWidgetSprites`): magic `"ZRB."` (a constant in the ELF), then U16 version 6. Compressed. Their records sit inside object data. **[code; the ELF bytes are confirmed]**
* Disc check (`UpdateCd`): sector 1256099 + 3232 = 1259331 (the first from a constant in the ELF), expected CRC `0xACDD3EE4` (reflected CRC-32, no final XOR). `info` and `extract` verify this on a real ISO. **[code; not run, since no ISO is available here]**

## Appendix: level table (identical in both samples) [C:verified]

World names come from the ELF's `g_apchzWorld`.

| # | world | ep | entry | lba | size | fid | list |
|---|---|---|---|---|---|---|---|
| 0 | 0 intro | 0 | 1 | 335488 | 9,969,830 | 0x5D | 64,65,67,68 |
| 1 | 0 intro | 1 | 0 | 350144 | 1,949,759 | 0x39 | 64,65,67,73 |
| 2 | 1 nightclub | 0 | 1 | 353856 | 10,938,886 | 0x41 | 64,74,67,68 |
| 3 | 1 nightclub | 2 | 0 | 377568 | 6,651,154 | 0x3B | 64,74,67,75 |
| 4 | 1 nightclub | 3 | 0 | 388768 | 7,645,080 | 0x3E | 64,74,67,76 |
| 5 | 1 nightclub | 4 | 0 | 417504 | 5,227,776 | 0x44 | 64,74,67,77 |
| 6 | 1 nightclub | 5 | 0 | 402976 | 5,751,263 | 0x47 | 64,74,67,78 |
| 7 | 1 nightclub | 6 | 0 | 409312 | 4,908,636 | 0x49 | 64,74,67,79 |
| 8 | 2 palace | 0 | 1 | 427328 | 11,689,480 | 0x52 | 64,80,67,68 |
| 9 | 2 palace | 7 | 0 | 444896 | 7,786,626 | 0x55 | 64,80,67,81 |
| 10 | 2 palace | 8 | 0 | 453760 | 6,104,199 | 0x4F | 64,80,67,82 |
| 11 | 2 palace | 9 | 0 | 465056 | 6,354,785 | 0x4C | 64,80,67,83 |
| 12 | 3 temple | 0 | 1 | 482752 | 12,361,630 | 0x57 | 64,84,67,68 |
| 13 | 3 temple | 7 | 0 | 509056 | 7,747,866 | 0x5A | 64,84,67,81 |
| 14 | 4 prison | 0 | 1 | 521344 | 11,383,853 | 0x82 | 64,85,67,68 |
| 15 | 4 prison | 7 | 0 | 540160 | 7,119,379 | 0x85 | 64,85,67,81 |
| 16 | 4 prison | 10 | 0 | 552768 | 3,737,282 | 0x88 | 64,85,67,86 |
| 17 | 5 castle | 0 | 1 | 561312 | 11,111,409 | 0x73 | 64,87,67,68 |
| 18 | 5 castle | 7 | 0 | - | 0 (empty) | - | 64,87,67,81 |
| 19 | 5 castle | 11 | 0 | 578848 | 4,208,012 | 0x7C | 64,87,67,88 |
| 20 | 5 castle | 12 | 0 | 603648 | 5,363,039 | 0x76 | 64,87,67,89 |
| 21 | 5 castle | 13 | 0 | 615360 | 4,588,804 | 0x7F | 64,87,67,90 |
| 22 | 5 castle | 4 | 0 | 589920 | 7,234,451 | 0x79 | 64,87,67,77 |
| 23 | 5 castle | 14 | 0 | 626688 | 4,456,544 | 0x6D | 64,87,67,91 |
| 24 | 5 castle | 15 | 0 | 649056 | 4,500,407 | 0x6A | 64,87,67,92 |
| 25 | 5 castle | 16 | 0 | 637920 | 4,273,600 | 0x67 | 64,87,67,93 |
| 26 | 5 castle | 17 | 0 | 660320 | 4,155,295 | 0x70 | 64,87,67,94 |
| 27 | 6 train | 0 | 1 | 671392 | 9,734,627 | 0x36 | 64,95,67,68 |
| 28 | 6 train | 18 | 0 | 687488 | 5,117,219 | 0x33 | 64,95,67,96 |
| 29 | 6 train | 19 | 0 | 698912 | 6,442,506 | 0x2D | 64,95,67,97 |
| 30 | 6 train | 20 | 0 | 710528 | 6,689,122 | 0x2F | 64,95,67,98 |
| 31 | 6 train | 21 | 0 | 720608 | 6,260,457 | 0x31 | 64,95,67,99 |
| 32 | 7 sawmill | 0 | 1 | 730464 | 10,965,082 | 0x24 | 64,100,67,68 |
| 33 | 7 sawmill | 22 | 0 | 749312 | 4,755,428 | 0x2A | 64,100,67,101 |
| 34 | 7 sawmill | 23 | 0 | 761664 | 5,074,218 | 0x21 | 64,100,67,102 |
| 35 | 7 sawmill | 24 | 0 | 773120 | 4,974,075 | 0x27 | 64,100,67,103 |
| 36 | 7 sawmill | 25 | 0 | 784544 | 3,715,670 | 0x1B | 64,100,67,104 |
| 37 | 7 sawmill | 26 | 0 | 795296 | 4,752,435 | 0x1E | 64,100,67,105 |
| 38 | 8 blimp | 0 | 1 | 808192 | 9,793,104 | 0x0F | 64,106,67,68 |
| 39 | 8 blimp | 7 | 0 | 823200 | 6,186,356 | 0x12 | 64,106,67,81 |
| 40 | 8 blimp | 27 | 0 | 845728 | 4,904,282 | 0x0C | 64,106,67,107 |
| 41 | 8 blimp | 28 | 0 | 834976 | 4,317,986 | 0x06 | 64,106,67,108 |
| 42 | 8 blimp | 29 | 0 | 856896 | 4,271,749 | 0x09 | 64,106,67,109 |
| 43 | 8 blimp | 26 | 0 | 867648 | 9,209,356 | 0x03 | 64,106,67,105 |

