typedef int qword __attribute__((mode(TI)));
struct VTE { short d; short p; void (*pfn)(void *); };
struct VTE5 { short d; short p; void (*pfn)(void *, int, int, int, int, int); };
struct VT { VTE a[3]; VTE5 a3; };
struct P { int n0; char pad[0x64 - 4]; int n64; };
struct S { int n0; int n4; int n8; VT *pvt; int n10; P *p14; int pad18[2]; qword q20; qword q30; int n40; int pad44; int n48; float f4C; int n50; int n54; int pad58[2]; };
struct SW { char pad[0x3D68]; int c; char pad2[0x3FFC - 0x3D6C]; char dl[0xC]; char pool[0x10]; };
extern SW D_003DD490;
extern VT D_002CE2B8;
struct CLOCK { float t; float dt; };
extern CLOCK D_002DDED0;
extern "C" {
S *PvAllocSlotheapClear(void *);
void *func_002333B4(void *, int, int);
void AppendDlEntry__FP2DLPv(void *, S *);
void FramePtr_Assign(S *, int);
}
struct Lock {
    S *p;
    Lock(S *pp) { p = pp; p->pvt->a[1].pfn((char *)p + p->pvt->a[1].d); }
    ~Lock() { p->pvt->a[2].pfn((char *)p + p->pvt->a[2].d); }
};
/** @sly2-annot addr=0x156548
 * Purpose: Creates a refcounted 0x60 stimulus object (cid 0x93) at a position from a template (class 0x75),
 *   stamps time/serial, appends it to the SW list and to D_003E148C
 * Info: proposed name NewNoise (T2); TU noise
 * Confidence: low (draft) [T2 0.35] - evidence: static analysis; sources: r3_1539A8
 * See: docs/engine/guards-ai.md#5-perception, docs/reference/functions/guards-ai.md#noise-0x1564e8-0x157200,
 *   docs/reference/functions/guards-ai.md#stepguard-0x205e70-0x20b840
 * @end-sly2-annot */
extern "C" void func_00156548(P *p, qword q, int b, S **pps)
{
    S *ps = PvAllocSlotheapClear(D_003DD490.pool);
    func_002333B4(ps, 0, 0x60);
    ps->n0 = 0;
    ps->n4 = 0;
    ps->p14 = p;
    ps->q20 = q;
    ps->q30 = q;
    ps->pvt = &D_002CE2B8;
    ps->n40 = b;
    ps->n48 = ++D_003DD490.c;
    ps->f4C = D_002DDED0.t;
    ps->n54 = p->n64;
    AppendDlEntry__FP2DLPv(D_003DD490.dl, ps);
    ps->pvt->a[1].pfn((char *)ps + ps->pvt->a[1].d);
    Lock l(ps);
    FramePtr_Assign(ps, p->n0);
    if (pps) *pps = ps;
    ps->pvt->a3.pfn((char *)ps + ps->pvt->a3.d, 2, 0, 0, 0, 0);
}
