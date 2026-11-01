struct VTE { short d; short pad; void (*pfn)(void *, int); };
struct O { char pad[0xC]; struct VTE *vt; };
struct S { char pad[0x24]; struct O *po; };
/** @sly2-annot addr=0x1ADE38
 * Purpose: small
 * Info: TU memcard
 * Notes: descriptive only: (McStatus helper) | no usable identifier; purpose only
 * Confidence: low (draft) [T2 0.20] - evidence: static analysis; sources: r4_17F090
 * @end-sly2-annot */
void func_001ADE38(struct S *p) { struct O *o = p->po; if (o) { struct VTE *e = &o->vt[49]; e->pfn((char *)o + e->d, -1); } }
