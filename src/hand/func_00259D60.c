struct S0 { char pad[0xE8]; int xE8; };
struct S1 { char pad[0xF64]; struct S0 *pF64; };
/** @sly2-annot addr=0x259D60
 * Purpose: return p->xF64->xE8 [game linkonce (inline/template)]
 * Info: proposed name GetF64_E8 (T2); TU game_linkonce
 * Confidence: low (draft) [T2 0.15] - evidence: static analysis; sources: sdk_names
 * See: docs/reference/functions/core-library.md#game_linkonce-0x254f00-0x259de4
 * @end-sly2-annot */
int func_00259D60(struct S1 *p) { return p->pF64->xE8; }
