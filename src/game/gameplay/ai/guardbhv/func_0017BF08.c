typedef int qword __attribute__((mode(TI)));
struct C { char pad[0x10]; void *p10; };
struct B { char pad[0xDC]; struct C *pDC; };
struct T { char pad[0xE8]; struct B *pE8; char pad2[0x294 - 0xEC]; int n294; char pad3[0xFE8 - 0x298]; int nFE8; };
struct S { char pad[0x10]; struct T *pt; };
void StartBhv(struct S *p);
qword func_001A62E8(void *p);
void func_001C02D0(void *p, qword a, qword b, int n);
void func_0017C550(struct S *p, int n);
/** @sly2-annot addr=0x17BF08
 * Purpose: BHVCIRCLE Start: with path-zone capability (bit 2) walk to a spacing point, else stand facing
 * Info: proposed name StartBhv179 (T2); class BHV179 slot8; TU guardbhv
 * Notes: BHV class word normalised: StartBhvcircle -> StartBhv179
 * Confidence: low (draft) [T2 0.45] - evidence: static analysis; sources: r9_bhv_screen
 * See: docs/reference/functions/guards-ai.md#guardbhv-0x174898-0x17f090
 * @end-sly2-annot */
void func_0017BF08(struct S *p) {
    StartBhv(p);
    if (p->pt->nFE8 & 4) {
        struct C *c = p->pt->pE8 ? p->pt->pE8->pDC : 0;
        void *pv = c ? c->p10 : 0;
        qword q = func_001A62E8(p->pt);
        func_001C02D0(pv, q, func_001A62E8(p->pt), p->pt->n294);
        func_0017C550(p, 1);
    } else {
        func_0017C550(p, 0);
    }
}
