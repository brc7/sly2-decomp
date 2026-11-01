typedef int qword __attribute__((mode(TI)));
struct VE { short d; short pad; void (*pfn)(); };
struct O { char pad[0xC]; struct VE *vt; char pad2[0x18]; unsigned long f28; };
struct T { char pad[0x180]; struct O *po; };
struct S { char pad[0x13F0]; struct T *pt; };
extern qword D_002C09D0, D_002A2DD0;
void func_001B7D20(struct S *p);
int func_00132B10(struct O *p, int n);
void func_00209FB8(struct O *p);
qword func_001A6FC0(struct S *p, int n, qword q);
void func_001F7A68(struct O *p, int n);
/** @sly2-annot addr=0x1B7AE8
 * Purpose: Release in state 8: detach, velocity = fixed local vector D_002C09D0, zero angular velocity,
 *   SetSoZapMode 3
 * Info: proposed name TossMurrayCarried (T2); TU murray
 * Confidence: low (draft) [T2 0.40] - evidence: static analysis; sources: r5b_rest
 * See: docs/reference/functions/player.md#murray-0x1b4728-0x1b9f80
 * @end-sly2-annot */
void func_001B7AE8(struct S *p)
{
    struct O *po;
    qword q;
    func_001B7D20(p);
    po = p->pt->po;
    p->pt = 0;
    if ((int)(po->f28 >> 60) & 1) {
        if (func_00132B10(po, 0x38)) func_00209FB8(po);
        q = func_001A6FC0(p, 0, D_002C09D0);
        ((void (*)(void *, qword))po->vt[35].pfn)((char *)po + po->vt[35].d, q);
        ((void (*)(void *, qword))po->vt[36].pfn)((char *)po + po->vt[36].d, D_002A2DD0);
        func_001F7A68(po, 3);
    }
}
