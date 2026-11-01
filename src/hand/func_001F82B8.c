struct K { int a, b; };
struct T { char pad[0x10]; int x; struct K k; };
struct T *func_001F8278(void *);
/** @sly2-annot addr=0x1F82B8
 * Purpose: Ensure object then set x and optional 8-byte key
 * Info: proposed name SetSoSideRecordParams (T2); TU so
 * Notes: verifier PLAUSIBLE: SetGlobalByIdParams -> SetSoSideRecordParams (exact field roles unknown) |
 *   verifier: purpose partly wrong: Find-or-create a 0xAC record in list D_003E145C keyed by the OBJECT
 *   POINTER (0x1F8240 compares rec+0xC to p; 0x20A5F8 inits int -1, floats 1.0/2.0), then set th
 * Confidence: low (draft) [T2 0.45] - evidence: static analysis; sources: r6_1D5150+verdicts_tu_r4r6
 * See: docs/engine/physics-collision.md#7-key-functions,
 *   docs/reference/functions/physics-collision.md#so-0x1f2108-0x1f8dd8,
 *   docs/reference/functions/player.md#stepplayer-0x13ff70-0x144638
 * @end-sly2-annot */
void func_001F82B8(void *p, int x, struct K *pk) { struct T *pt = func_001F8278(p); pt->x = x; if (pk) pt->k = *pk; }
