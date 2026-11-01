struct VTE { short d; short pad; int (*pfn)(void *, int); };
struct O { char pad[0xC]; struct VTE *vt; };
struct S { struct O *po; };
/** @sly2-annot addr=0x1A2358
 * Purpose: Return owner vt86(record)
 * Info: proposed name FJtVetoDamage (T2); TU hurt
 * Confidence: low (draft) [T2 0.30] - evidence: static analysis; sources: r4b_rest
 * See: docs/reference/functions/player.md#hurt-0x1a0b18-0x1a2bd0
 * @end-sly2-annot */
int func_001A2358(struct S *p, int a) { struct O *o = p->po; struct VTE *e = &o->vt[86]; return e->pfn((char *)o + e->d, a); }
