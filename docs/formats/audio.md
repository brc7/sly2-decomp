# Audio files: 989snd banks, VPK streams, VAGp dialogue

Decoder: `reports/audio/sly2_audio.py` (stdlib + numpy; exports WAV with loop points and spectrograms; guides/asset-tools.md).
Engine side: [sound](../engine.md#77-sound-enginesound-srcsdk989snd).

Tags: `[C:parse]` = header fields account exactly for the file size or tables; `[C:decode]` = the decoded signal passes plausibility checks (speech formants,
tonal music, no clipping); `[C:static]` = IOP or EE disassembly; `[H]` = inferred. **Only one sample per kind was decoded**; the other files are presumed to use the
same formats `[H]`.

## 1. File kinds

| manifest kind | Container | Content | Tag |
|---|---|---|---|
| sound_bank | 989snd bank, 'SBlk' version 3 | per-level sound effects | [C:parse+decode] |
| sound_ambience | VPK (`" KPV"`), N mono channels, 32 kHz | ambience beds | [C:static+parse+decode] |
| sound_file | VPK, 44.1 kHz, layered stems | music | [C:static+parse+decode] |
| sound_stream | VAGp, mono, 22050 Hz | dialogue | [C:static+decode] |

## 2. The IOP stream module `spiop.irx` ('SUKR')

The ELF embeds 9 IRX modules (sio2man 0x1DD380, padman 0x1DEDC0, mcman 0x1E9D80, mcserv 0x201480, libsd 0x2031C0, usbd 0x20A200, lgaud 0x212B00, 989snd 0x21ED80,
**spiop 0x24A5C0** (file offsets)). spiop is Sucker Punch's stream module and RPC server 'SUKR' (0x5355<<16 | 0x4B52); its stream-open routine (.text 0x20C4)
branches on the first sector's magic `[C:static]`:
- **VAGp**: data at 0x30, 1 channel, size = bswap(+0x0C); a VAG path also requires bswap(+0x10) == 0x5622 (22050 Hz).
- **VPK**: requires +0x0C == 0x2000 and channels (+0x14) < 6; per channel an SPU ring of 0xC000 bytes and a synthesised 0x30-byte VAGp header passed down.

## 3. VAGp (dialogue)

`+0x00 "VAGp"; +0x04 u32BE version (0); +0x0C u32BE data bytes; +0x10 u32BE sample rate (22050); +0x20 name[16]; +0x30 PS-ADPCM mono`. Sample `stream1259_lang0`:
0x30 + 0xF080 = the file size; 4.886 s of clear speech `[C:parse+decode]`. The first frame is a zero frame.

## 4. VPK (ambience and music)

| Offset | Field | Tag |
|---|---|---|
| +0x00 | `" KPV"` | |
| +0x04 | u32 bytes per channel | [C:static+parse] |
| +0x08 | u32 data offset (0x800) | [C:static+parse] |
| +0x0C | u32 0x2000 (required); **actual interleave = 0x1000 bytes per channel** | [C:parse+decode] (block-boundary continuity, stereo correlation 0.92 at 0x1000 vs ~0 otherwise; agrees with vgmstream) |
| +0x10 | u32 sample rate (32000 ambience, 44100 music) | [C:static+decode] |
| +0x14 | u32 channels (<= 5) | [C:static] |
| +0x18 + 4i | per channel: u8 vol (0x7F), s8 group, s16 pan in degrees (0 centre, 270 left, 90 right) | group/pan read by the IOP [C:static]; meanings [H] |
| +0x7FC | u32 loop start as a per-channel byte offset (0 = no loop); loop end = end of stream | read [C:static], semantics [H] |

From 0x800 the channels' 0x1000-byte blocks interleave round-robin; the last block is padded with silent frames. Samples: L02 ambience = 4 independent mono beds in
groups 0-3 (120 s, no loop); `sndfile28` = mono stem (group 0) + two stereo pairs (groups 1, 2), loop at 14.9 s of 96.9 s: music layers the game crossfades `[H]`
(cf. the SNDMGR music layer mix). The decoder writes one WAV per group (stereo for 270/90 pairs) with a RIFF `smpl` loop.

## 5. 989snd bank ('SBlk' v3)

File: `u32 3 (type); u32 nchunks (2); nchunks x {u32 offset, u32 size}`: chunk 0 = SBlk header/tables, chunk 1 = SPU sample image; the chunks end at the file size
`[C:parse]` (same outer header as Sly 1's `Parse989SfxBank`).

SBlk (offsets relative to chunk 0): `+0 "SBlk"; +4 version 3; +8 4; +0xC bank id; +0x16 u16 nsound (383); +0x18 u16 ngrain (3028); +0x1A u16 nwave (460);
+0x1C sound table offset; +0x20 grain table offset; +0x24 0x5040 (?); +0x28 sample size; +0x2C SPU RAM size; +0x30 next block; +0x34 grain-data offset; +0x38 names (0)`
`[C:parse]` (offsets chain exactly).
- **Sound** (12 B): s8 vol, s8 volgroup, s16 pan, s8 ngrain, s8 instance limit, u16 flags (bit 0 looper), u32 first grain offset (Sly 1 SOUND layout) `[C:parse]`.
- **Grain** (8 B): u32 `type << 24 | data offset`, s32 delay. Only type 1 (TONE) carries audio; types 4, 5, 20-31, 39 are control grains (not analysed).
- **TONE** (0x18 B): s8 priority, s8 vol, u8 center_note, u8 center_fine, s16 pan, s8 map_lo/hi, s8 pb_lo/hi, u16 ADSR1, u16 ADSR2, u16 flags, u32 sample offset (into
  chunk 1), u32 size (0 in Sly 2); 460 distinct offsets = nwave `[C:parse]`.
- Rate ~ `48000 * 2^(((60 - (256 - center_note)) + center_fine/128) / 12)` for center_note >= 0x80: mostly ~11025 Hz, some ~22050 `[H]` (vgmstream/SCREAM formula).
- Waveforms in chunk 1: each starts with a 16-byte zero frame; non-loopers end with an end-flag frame + one pad frame (445/445); the 15 loopers start the loop at frame 1
  (flag 4) and end with flag 3 `[C:parse]`.
- Mapping to game SFX ids: the world file's sound section maps `aisnd[sfxid]` -> sound index (`LoadSoundSectionFromBrx` 0x1FD0E8); not joined yet.

## 6. PS-ADPCM

16-byte frames: byte 0 = filter << 4 | shift, byte 1 = flags (bit 0 end, bit 1 repeat, bit 2 loop start), 14 data bytes = 28 nibbles (low first).
`s = (int16)(nib << 12) >> shift; s += (h1*c0 + h2*c1 + 32) >> 6`, coefficients {0,0},{60,0},{115,-52},{98,-55},{122,-60} `[C:decode]` (bit-exactness vs SPU2 not checked).

## Devtools findings (v3, 2026-11-01)

- devtools/audio_browser decodes every manifest kind (VAGp 1,691, VPK 100, SBlk 43, PSS 2); PSS movie audio = SShd type 0x10 PS-ADPCM, 44.1 kHz,
  2 ch, interleave 0x20 `[C:decode]` (one movie checked). Signal self-tests 23/23 with negative controls (e.g. a 6-byte misaligned dialogue decode is flagged).
- Bank L02: 460/460 waves end exactly at their end flag; 383 sounds vs 381 sfx ids mapped `[C:parse]`.
- 989snd grain types other than 1 (TONE) are not modelled (types 4, 5, 20-31, 39 occur) - open. Odd-rate tones (> 48 kHz from the note-60 formula) `[H]`.
- Stream "pairs" (8-byte records `{u16 dt, u16 5, u32 0x14FC..0x1501}`) look like lip-sync keys `[H]`.
- `g50` = stream duration in seconds is now confirmed ([world-file](world-file.md)).

## 7. Open

- Bank -> SFX id join; non-TONE grain semantics; 989snd pitch/ADSR playback (exports are raw samples, not full SFX renders).
- SUKR commands 0-12; VPK per-channel byte 0 and +0x18 flags.
- A batch run over all 1,880 manifest files to check every file.
