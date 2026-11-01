void LoadSoFromBrx(void *a, void *b);
void CBinaryInputStream_SkipStringSw(void *b);
/** @sly2-annot addr=0x1C3DC8
 * Purpose: PO::LoadFromBrx (vt slot 14): LoadSoFromBrx (0x1F31E0) then one stream helper 0x118420
 * Info: proposed name LoadPoFromBrx (T1); class PO slot14; TU po
 * Notes: level_parser CONFIRMED-parse: payload layout followed by the parser, 100%% of bytes in 4 levels
 * Confidence: high [T1 0.85] - evidence: static analysis; sources: r5_1AE130+parse-verified
 * See: docs/engine/player.md#11-key-functions,
 *   docs/formats/level-objects.md#5-per-class-payloads-vtable-slot-14,
 *   docs/reference/functions/player.md#po-0x1c3d70-0x1c4e50
 * @end-sly2-annot */
void LoadPoFromBrx(void *a, void *b) { LoadSoFromBrx(a, b); CBinaryInputStream_SkipStringSw(b); }
