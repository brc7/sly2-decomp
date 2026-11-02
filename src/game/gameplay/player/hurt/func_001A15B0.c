struct K { int a, b; };
struct S { char pad[0x1C]; struct K k; };
/** @sly2-annot addr=0x1A15B0
 * Purpose: Store a pointer-to-member callback (pmf) to run when the hurt animation ends
 * Info: proposed name SetJtHurtCallback (T2); TU hurt
 * Confidence: low (draft) [T2 0.45] - evidence: static analysis; sources: r4b_rest
 * See: docs/reference/functions/player.md#hurt-0x1a0b18-0x1a2bd0
 * @end-sly2-annot */
void func_001A15B0(struct S *p, struct K k) { p->k = k; }
