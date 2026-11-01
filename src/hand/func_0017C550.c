struct VTE { short d; short pad; void (*pfn)(void *); };
struct O { char pad[0xC]; struct VTE *vt; char pad2[0x28]; int n38; char pad3[0xC0 - 0x3C]; int nC0; int pad4; int nC8; };
struct X { char pad[0xC]; int nC; };
struct T { char pad[0x294]; int n294; char pad2[0x3CC - 0x298]; struct X *px; };
struct S { char pad[0x10]; struct T *p10; int pad2; char a18[0x18]; int n30; };
void func_001EF0D8(void *p);
struct O *func_00205438(void *p, int n);
void func_00178D90(struct O *o, int a, int b);
/** @sly2-annot addr=0x17C550
 * Purpose: BHVCIRCLE state: 0 push BHVIDLE facing target, 1 push BHVWALKPATH on the guard path shape
 * Info: proposed name SetBhv179State (T2); TU guardbhv
 * Notes: BHV class word normalised: SetBhvcircleState -> SetBhv179State
 * Confidence: low (draft) [T2 0.45] - evidence: static analysis; sources: r9_bhv_screen
 * See: docs/reference/functions/guards-ai.md#guardbhv-0x174898-0x17f090
 * @end-sly2-annot */
void func_0017C550(struct S *p, int n) {
    if (n == p->n30) return;
    func_001EF0D8(p->a18);
    p->n30 = n;
    switch (n) {
    case 0:
        { struct O *o = func_00205438(p->p10, 0x9E); struct VTE *e; o->n38 = p->p10->px->nC; e = &o->vt[8]; e->pfn((char *)o + e->d); }
        break;
    case 1:
        { struct O *o = func_00205438(p->p10, 0xA7); struct VTE *e; func_00178D90(o, p->p10->n294, 0); o->nC0 = 8; o->nC8 = p->p10->px->nC; e = &o->vt[8]; e->pfn((char *)o + e->d); }
        break;
    }
}
