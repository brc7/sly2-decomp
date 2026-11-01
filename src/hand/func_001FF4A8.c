struct K { int a, b; };
struct S { char pad[0x104]; struct K k; };
void func_002311B0(struct S *);
/** @sly2-annot addr=0x1FF4A8
 * Purpose: Clone: XFM clone (0x2311B0) preserving a key field
 * Info: proposed name CloneCid39 (T2); class CID39 slot41; TU tu_1FF3B8
 * Notes: verifier WRONG: CloneCid27 -> CloneCid39
 * Confidence: low (draft) [T2 0.80] - evidence: static analysis; sources: r7_1F8DD8+verdicts_tu_r7r8
 * See: docs/engine/gameplay-objects.md#key-functions,
 *   docs/reference/functions/gameplay-objects.md#tu_1ff3b8-0x1ff3b8-0x1ff690
 * @end-sly2-annot */
void func_001FF4A8(struct S *p) { struct K k = p->k; func_002311B0(p); p->k = k; }
