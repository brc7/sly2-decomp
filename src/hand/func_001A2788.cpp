typedef int qword __attribute__((mode(TI)));
typedef union { qword q; float f[4]; } U;
struct FRAME { int a[4]; FRAME(); ~FRAME(); };
struct MSG { int cb; int nKind; };
struct HIT { qword pos; char pad[0x5F0]; };
struct O { char pad[0x1098]; float x1098; float x109C; };
struct L { char pad[0xE0]; int xE0; };
extern "C" {
extern qword D_002A2DD0;
qword func_001A62E8(void *p);
void *func_001AE318(MSG *pmsg);
int func_001F7E08(L *pl, void *pfn, int n, void *pv);
int func_0014CDB0(qword a, qword b, int n, void *pv, int c, HIT *phit);
void func_00229D60(void);
int func_001A2788(O *po, L *pl, qword *pv);
}
/** @sly2-annot addr=0x1A2788
 * Purpose: Line/part test from the player down to the linked volume's surface; out position
 * Info: proposed name FProbeStepplayerSurface (T2); TU hurt
 * Confidence: low (draft) [T2 0.30] - evidence: static analysis; sources: r4b_rest
 * See: docs/reference/functions/player.md#hurt-0x1a0b18-0x1a2bd0
 * @end-sly2-annot */
int func_001A2788(O *po, L *pl, qword *pv)
{
    U off, b, a;
    HIT hit;
    off.q = D_002A2DD0;
    {
        qword p = func_001A62E8(po);
        qword d;
        __asm__("vsub.xyzw %0, %1, %2" : "=j"(d) : "j"(p), "j"(off.q));
        a.q = d;
        b.q = d;
    }
    a.f[2] += po->x109C;
    b.f[2] += po->x1098;
    FRAME frame;
    MSG msg;
    MSG *pmsg = &msg;
    pmsg->cb = pl->xE0 * 4;
    pmsg->nKind = 0x10;
    void *pbuf = func_001AE318(pmsg);
    int n = func_001F7E08(pl, (void *)func_00229D60, 0, pbuf);
    if (!func_0014CDB0(a.q, b.q, n, pbuf, 0x10, &hit))
        return 0;
    qword r;
    __asm__("vadd.xyzw %0, %1, %2" : "=j"(r) : "j"(hit.pos), "j"(off.q));
    *pv = r;
    return 1;
}
