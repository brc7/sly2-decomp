typedef int qword __attribute__((mode(TI)));
struct S;
struct VE { short d; short pad; void (*pf)(void *, int, void *, qword, qword, qword); };
struct S { char pad[0xC]; struct VE *vt; char pad2[0x218 - 0x10]; qword *p218; char pad3[0xF80 - 0x21C]; int k; void *pF84; float fF88; int pad4; void *pF90; };
extern qword D_002A2DA0;
void func_001F4FA8(struct S *p);
void func_001E6F10(void *po, qword *pa, qword *pb, float f);
qword func_001A62E8(void *p);
qword *func_001A6318(void *p);

/** @sly2-annot addr=0x209C88
 * Purpose: STEPGUARD vt59: when attached (+0xF80 0..3) add constraint contacts via vt82 against the attach
 *   object (rail/pipe closest point 0x1E6F10, target pos, fixed axis)
 * Info: proposed name AddStepguardAttachXps (T2); class STEPGUARD slot59; TU stepguard
 * Confidence: low (draft) [T2 0.45] - evidence: static analysis; sources: r7b_guard
 * See: docs/reference/functions/guards-ai.md#stepguard-0x205e70-0x20b840
 * @end-sly2-annot */
void func_00209C88(struct S *p)
{
    qword a, b;
    func_001F4FA8(p);
    switch (p->k) {
    case 0:
        func_001E6F10(p->pF84, &a, &b, p->fF88);
        p->vt[82].pf((char *)p + p->vt[82].d, 0x22, p->pF84, a, b, *p->p218);
        break;
    case 1: {
        struct VE *pe;
        void *pthis;
        func_001E6F10(p->pF84, &a, &b, p->fF88);
        pe = &p->vt[82];
        pthis = (char *)p + pe->d;
        pe->pf(pthis, 7, p->pF84, a, b, func_001A62E8(p->pF90));
        break;
    }
    case 2: {
        struct VE *pe = &p->vt[82];
        void *pthis = (char *)p + pe->d;
        pe->pf(pthis, 7, p->pF84, func_001A62E8(p->pF84), *func_001A6318(p->pF84), func_001A62E8(p->pF90));
        break;
    }
    case 3: {
        struct VE *pe = &p->vt[82];
        void *pthis = (char *)p + pe->d;
        pe->pf(pthis, 7, p->pF84, func_001A62E8(p->pF84), D_002A2DA0, *p->p218);
        break;
    }
    }
}
