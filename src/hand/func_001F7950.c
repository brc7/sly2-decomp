typedef int qword __attribute__((mode(TI)));
typedef unsigned long ulong;
struct VTE { short d; short p; void *pfn; };
struct VT { struct VTE a[75]; };
struct E { char pad[0x6C]; int n6C; };
struct P { char pad[0xC]; struct VT *pvt; char pad2[0xEC - 0x10]; struct E *pe; char pad3[0x148 - 0xF0]; union { struct { signed char b0, b1; } c; ulong l; } u; };
struct M { void *p0; int n4; void *p8; int nC; int n10; int pad[3]; qword q20; qword q30; char pad2[0x60 - 0x40]; };
struct G2 { void *p; };
extern struct G2 D_002F7900;
void func_001F7A68(struct P *, int);
void func_002333B4(void *, int, int);
qword func_001A62E8(struct P *);
int FIsBasicDerivedFrom__FP5BASIC3CID(void *, int);
void func_001F7598(struct M *);
/** @sly2-annot addr=0x1F7950
 * Purpose: Deferred callback: process contact zaps (vt74) then emit self zap depending on zap mode (+0x148
 *   bits 24-25)
 * Info: proposed name SoTouchZapCallback (T2); TU so
 * Confidence: low (draft) [T2 0.30] - evidence: static analysis; sources: r6_1D5150
 * See: docs/reference/functions/physics-collision.md#so-0x1f2108-0x1f8dd8
 * @end-sly2-annot */
void func_001F7950(struct P *p)
{
    int k;
    struct M m;
    if (!p->pe) return;
    if (p->pe->n6C) ((void (*)(void *))p->pvt->a[74].pfn)((char *)p + p->pvt->a[74].d);
    if (!p->pe) return;
    k = (int)(p->u.l >> 24) & 3;
    if (!k) return;
    func_001F7A68(p, 0);
    func_002333B4(&m, 0, 0x60);
    m.p8 = p;
    m.q20 = func_001A62E8(p);
    m.q30 = ((qword (*)(void *))p->pvt->a[33].pfn)((char *)p + p->pvt->a[33].d);
    if (k == 3 && FIsBasicDerivedFrom__FP5BASIC3CID(p, 0x38)) {
        m.nC = 0x40000; m.n10 = -50; m.p0 = D_002F7900.p;
    } else {
        m.p0 = p; m.nC = 0x20000; m.n10 = p->u.c.b1;
    }
    func_001F7598(&m);
}
