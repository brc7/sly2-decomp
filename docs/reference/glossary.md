# Glossary: Sucker Punch naming and common abbreviations

Sly 1 and Sly 2 use a compact Hungarian naming style inherited from Sucker Punch's codebase. Sly 1 shipped symbols (via the
ProjectCane PC port and the Sly 1 decomp), so its spellings are the best source for Sly 2 names. Names below marked **(team)** were coined
during this project and are not Sucker Punch's.

## 1. Prefixes in function and variable names

| Prefix | Meaning | Example |
|---|---|---|
| `P` / `p` | returns / is a pointer to | `PloNew` (returns LO*), `PosLoWorld` (returns position), `psoParent` |
| `F` / `f` | boolean (function returns, or flag) | `FIsLoInWorld`, `fClosed` |
| `C` / `c` | count | `CploFindSwObjects` (returns a count), `cwe`, `csnd` |
| `G` / `g` | float value ("g" = general real) | `GSmooth`, `gAlpha`, `gProbability` |
| `I` / `i` | index | `IdProxy`, `iwe`, `isplice` |
| `N` / `n` | integer number | `NScaleByDifficulty`, `nState` |
| `A` / `a` | array | `axf`, `aoid`, `apbhv` |
| `Grf` / `grf` | flag bitfield | `grfvis`, `GrfPressedWithin` |
| `Mp` / `mp` | map (array indexed by an enum) | `mplootkc` (loot count per kind), `mppukfAcquired` |
| `Rad` / `rad` | angle in radians | `radTarget`, `RadNormalize` |
| `S` / `s` | distance (length) | `sBusy`, `sNearClip`, `sRadius` |
| `T` / `t` | time in seconds | `tSplice`, `tWipe` |
| `Dt` / `dt` | time delta | `DtBallisticMax` |
| `U` / `u` | normalized 0..1 parameter | `uAlpha`, `uZoom` |
| `Pchz` / `pchz` | pointer to zero-terminated string | `PchzFindSwLangstr` |
| `Ref` | returns a Splice `CRef` | `RefEval`, `RefOpAdd` |
| `Pv` | returns `void*` | `PvAllocSlotheapClear` |
| `Clone`, `Init`, `Load...FromBrx`, `Post...Load`, `Update`, `Render...Self`, `On...Add/Remove` | lifecycle verbs used per class | `LoadSoFromBrx`, `PostLoLoad`, `OnPipeAdd` |
| `g_` | global | `g_sw`, `g_cm`, `g_clock`, `g_pjt` |
| `s_` | file static | `s_aotypEvtParm` |
| `D_XXXXXXXX` | unnamed global at that address (decomp convention) | `D_003DD490` = `g_sw` |
| `func_XXXXXXXX` | unnamed function (decomp convention) | |

Methods are named `<Verb><Class><Noun>`: `SetJtJts` = "set JT's JTS (state)", `PloFindSwObject` = "pointer to LO: find SW object".

## 2. Classes and structures

| Abbr. | Meaning |
|---|---|
| BASIC | root class of every scripted object (Splice frame, sidebag, subscribers, vptr) |
| LO | Level Object: anything placed in a level |
| XFM | transform object: LO + its own matrix (base of lightweight objects) |
| SO | Solid Object: physical, collidable, has a model |
| PO | Playable/Player Object (controllable by a POCTL) |
| STEP | walking character base |
| STEPPLAYER | shared base of the three player characters (new in Sly 2) |
| JT | Sly Cooper's class (Sly 1 legacy name) |
| STEPGUARD | guard base class |
| SW | the world (one per loaded level) |
| CM | camera |
| CPLCY, CPLOOK, CPZOOM, CPTN, ... | camera policies |
| CPDEFI | camera definition supplied by the focus object |
| BHV, BHVSTK | behaviour object, per-character behaviour stack (new in Sly 2) |
| SGF | guard flag (bit in STEPGUARD+0x2C0) |
| XF | transform record inside an SO (matrix + world matrix + role) |
| SOPART, SOPHYS | SO collision part, SO physics record |
| GLOBSET, GLOB, SUBGLOB | an SO's model and its sub-models |
| SAA | shader animation (LOOP, PINGPONG, SHUFFLE, EYES, SCROLLER, CIRCLER) |
| ASEG, ASEGA, ASEGBL | animation segment, its application to an object, a blended segment |
| SM, SMA, SMS, SMT | state machine, its application, state, transition |
| RSMG | "run state machine goal" record: (root, SM, goal) triggered by events |
| ACT | actuator (animated channel driver) |
| ZPK, ZPR | zap kind (damage type bits), zap record (damage delivery) |
| XP | contact point between two SOs |
| DZG | the contact constraint solver (LCP) |
| BSP, BSPC | binary space partition (collision / zones), BSP container |
| GEOM | point + plane mesh used for collision |
| VOL | volume (trigger/containment region) |
| TN | tunnel/zone volume driving the CPTN camera |
| HND | hook the player can swing from |
| JMT | jump target |
| PATHZONE, CG | navigation zone, its navigation graph |
| PROXY, PROXYSRC, PROXYSPARE | proxy object, proxy source (world entry), spare copy |
| WE | world entry (= proxy source record in the level file) |
| LOROOT | per-hierarchy root record (activation, busy list) |
| RIP, RIPG | particle, particle group |
| EMITTER, EMITB | particle emitter, emitter parameters |
| EXPL, EXPLG, EXPLO, EXPLS | explosion family |
| BLIPG | sprite particle group |
| DPRIZE | dropped prize (coins, pickups) |
| WIDGET | HUD element (Sly 1: BLOT) |
| RELAY, JSG, RELAYCMD, RELAYCK | scripted-sequence object, its interpreter, a command, the command kind |
| JOB, TASK | mission and mission step **(team names)** |
| GS (`g_gs`) | game state: the save blob (in Sly 1 GS also = Graphics Synthesizer, see below) |
| SAVE | memory-card save state machine |
| SND, SNDMGR, BANKSND, STREAMSND | sound instance, manager, bank sound, streamed sound |
| POCTL | player-object controller (filtered pad state) |
| FRM | a frame being built for the render thread |
| RPL | render-list entry |
| GSB | GS memory block allocator |
| FILEREC | 64-byte disc file record **(team name)** |

## 3. Identifiers and data

| Abbr. | Meaning |
|---|---|
| CID | class id: index into the class-descriptor table D_002BAAE8 (199 in Sly 2) |
| OID | object id (32-bit in Sly 2 level files; also used as template/widget/animation ids) |
| EOPID, OPTID, OTYP | option (property) id, Sly 1 option id, option value type |
| SYMID | Splice symbol id |
| BIF, BIFK | Splice builtin function, builtin index |
| TAGK | Splice value tag |
| SFXID | sound effect id |
| JTS, JTBS, JTCS | Sly's state, sub-state, cane state |
| BRX | level ("world") file format and its loaders (`Load...FromBrx`) |
| BIS | binary input stream (`CBinaryInputStream`) |
| LM, CLQ, SMP | limits (min/max), quadratic coefficients, smoothing parameters (Splice value kinds) |
| DL/DLE, SL/SLE | doubly / singly linked list and entry |

## 4. PS2 terms

| Term | Meaning |
|---|---|
| EE | Emotion Engine: the main MIPS R5900 CPU (runs all game code) |
| IOP | I/O processor (PS1 CPU): CD, sound, memory card; talked to via SIF RPC |
| IRX | IOP module (e.g. Sucker Punch's 'SUKR' streaming module, 989snd) |
| VU0 / VU1 | vector units: VU0 is used as a coprocessor for math/culling, VU1 runs vertex microprograms |
| VIF / GIF | interfaces that feed VU1 / the GS from DMA |
| GS | Graphics Synthesizer: the rasterizer with 4 MB of local memory |
| DMA chain | linked list of DMA tags (CNT, REF, CALL, RET...) built by the EE |
| Scratchpad | 16 KB fast RAM at 0x70000000 (used as the LZSS window) |
| IPU | MPEG decoder hardware (FMVs) |
| LBA / sector | 2048-byte disc sector index |
