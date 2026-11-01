typedef int qword __attribute__((mode(TI)));
struct A { char pad[0xA0]; int nA0; };
struct O { char pad[0x10]; struct O *pParent; };
struct L { char pad[0x30]; qword v; };
struct S { char pad[0x10]; struct O *pParent; int pad14; int cid; char pad2[0x7C - 0x1C]; int a, b, c; char pad3[0x90 - 0x88]; qword v90;
           struct A *pa; int nA4; int pad4[1]; struct O *pAC; char eB0[4]; int hB4; char pad5[0xC0 - 0xB8]; void *pC0; };
void func_001A5C48(struct S *p);
int func_00132B10(struct O *o, int n);
void func_00209130(void *pe, struct O *o);
void *func_001F4C58(struct O *o, int a, int b);
float *func_0012F938(int h, int a);
void func_001300B8(int h, struct O *o, int a, int b, struct L *pl, int c, float f, float g);
/** @sly2-annot addr=0x20AE18
 * Purpose: cid 0x78 vt15: PostLoLoad, find owning STEPGUARD up parent chain, resolve binding, precompute ASEG
 *   effect (0x1300B8)
 * Info: proposed name PostCid120AttackLoad (T2); class CID120 slot15; TU stepguard
 * Notes: Cid78->Cid120 (CID 0x78=120, vtable)
 * Confidence: low (draft) [T2 0.40] - evidence: static analysis; sources: r7b_guard
 * See: docs/reference/functions/guards-ai.md#stepguard-0x205e70-0x20b840
 * @end-sly2-annot */
void func_0020AE18(struct S *p)
{
    struct O *o;
    func_001A5C48(p);
    for (o = p->pParent; o && !func_00132B10(o, 0x38); o = o->pParent)
        ;
    p->pAC = o;
    func_00209130(p->eB0, o);
    if (p->cid != 0x2CC)
        p->pC0 = p->eB0;
    if (p->a != -1) {
        struct L l;
        int h;
        float *pf;
        p->pa = func_001F4C58(p->pAC, p->a, 6);
        p->nA4 = (int)func_001F4C58(p->pAC, p->b, 6);
        h = p->hB4;
        pf = func_0012F938(h, p->c);
        func_001300B8(h, p->pAC, p->nA4, p->pa->nA0, &l, 0, *pf, 1.0f);
        p->v90 = l.v;
    }
}
