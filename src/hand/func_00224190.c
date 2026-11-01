struct VTE { short d; short pad; void (*pf)(void *, int); };
struct S { int a[3]; struct VTE *vt; char pad[0x140]; int x; };
/** @sly2-annot addr=0x224190
 * Purpose: if state in {1,2} SetState(3); return 1
 * Info: class CID100 slot75; TU tu_223F70
 * Notes: descriptive only: Cid100 state helper: drop to state 3 from 1/2 | no usable identifier; purpose only
 * Confidence: low (draft) [T2 0.30] - evidence: static analysis; sources: r8_21BB88
 * @end-sly2-annot */
int func_00224190(struct S *p)
{
    switch (p->x) { case 1: case 2: p->vt[76].pf((char *)p + p->vt[76].d, 3); }
    return 1;
}
