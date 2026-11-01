struct T { char pad[0xAC]; unsigned short n; };
struct S { char pad[0x24]; struct T *pt; };
/** @sly2-annot addr=0x1F8870
 * Purpose: duplicate of 0x1F87A0 (different vtable)
 * Info: proposed name GetXfhParamAC2 (T2); TU so
 * Notes: verifier: purpose partly wrong: Getter: XF-handle (+0x20 SO, +0x24 XF) -> u16 XF+0xAC * 0.001
 *   (ms->s); setter 0x1F8898. Code identical to 0x1F87A0, but the two refs are Splice property descrip
 * Confidence: low (draft) [T2 0.15] - evidence: static analysis; sources: r6_1D5150+verdicts_tu_r4r6
 * See: docs/engine/physics-collision.md#7-key-functions,
 *   docs/reference/functions/physics-collision.md#so-0x1f2108-0x1f8dd8
 * @end-sly2-annot */
float func_001F8870(struct S *p) { return p->pt->n * 0.001f; }
