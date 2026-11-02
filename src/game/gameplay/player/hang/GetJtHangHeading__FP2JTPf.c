struct VTE { short d; short pad; float *(*pfn)(void *); };
struct O { int a, b, c; struct VTE *vt; };
struct S { char pad[0x18C4]; struct O *o; };
float atan2f(float, float);
/** @sly2-annot addr=0x190BE0
 * Purpose: *prad = atan2(m[0].y, m[0].x) of the current hook object's (x18C4) world matrix
 * Info: proposed name GetJtHangHeading (T1); TU hang
 * Confidence: high [T1 0.85] - evidence: static analysis; sources: r4b_rest
 * See: docs/reference/functions/player.md#hang-0x18f908-0x191090
 * @end-sly2-annot */
void GetJtHangHeading__FP2JTPf(struct S *p, float *pg) {
    float *a, *b;
    { struct O *o = p->o; struct VTE *e = &o->vt[31]; a = e->pfn((char *)o + e->d); }
    { struct O *o = p->o; struct VTE *e = &o->vt[31]; b = e->pfn((char *)o + e->d); }
    *pg = atan2f(a[1], b[0]);
}
