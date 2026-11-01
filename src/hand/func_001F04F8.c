struct E { int a; int b; };
struct S { char pad[0x50]; int c; struct E *ae; };
/** @sly2-annot addr=0x1F04F8
 * Purpose: asms[isms].oid, or -1 if out of range
 * Info: proposed name OidFromSmIsms (T1); TU sm
 * Notes: names_v2 v1 proposed IsmsFindSmRequired__FP2SM3OID (0.001)
 * Confidence: high [T1 0.85] - evidence: semantic review; static naming pass; sources: semantic_tu_docs/sm
 * See: docs/engine/animation.md#7-key-functions, docs/reference/functions/animation.md#sm-0x1efa90-0x1f1768
 * @end-sly2-annot */
int func_001F04F8(struct S *p, int n) { if (n < 0 || n >= p->c) return -1; return p->ae[n].a; }
