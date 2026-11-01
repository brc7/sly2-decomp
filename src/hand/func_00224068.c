struct VTE { short d; short pad; void (*pf)(void *, int); };
struct S { int a[3]; struct VTE *vt; char pad[0x158]; char y[4]; };
void RemoveSoHierarchy(struct S *p);
void ReleaseSma(void *d);
/** @sly2-annot addr=0x224068
 * Purpose: SO slot12, SetState(-1) via vt76, release SM handle 0x1F15B8
 * Info: proposed name RemoveCid100Hierarchy (T2); class CID100 slot12; TU tu_223F70
 * Notes: agent qualifier: (slot12)
 * Confidence: low (draft) [T2 0.40] - evidence: static analysis; sources: r8_21BB88
 * See: docs/reference/functions/gameplay-objects.md#tu_223f70-0x223f70-0x2259d8
 * @end-sly2-annot */
void func_00224068(struct S *p)
{
    RemoveSoHierarchy(p);
    p->vt[76].pf((char *)p + p->vt[76].d, -1);
    ReleaseSma(p->y);
}
