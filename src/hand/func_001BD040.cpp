typedef int qword __attribute__((mode(TI)));
typedef struct { qword r[4]; } M4;
struct CTX { int a[4]; CTX(); ~CTX(); };
struct AREQ { int cb; int align; AREQ(int c, int a) { cb = c; align = a; } };
struct X;
typedef struct { short d; short pad; M4 *(*pfn)(X *); } VT;
union U { qword q; float f[4]; };
struct X { int x0, x4, x8; VT *vt; char pad[0xE8 - 0x10]; int c; char pad2[0xF4 - 0xEC]; qword *apos;
           char pad3[0x1A0 - 0xF8]; U vmin; U vmax; };
extern "C" {
void *func_001AE318(const AREQ &r);
qword func_001A62E8(X *p);
void *func_001C0D70(qword pos, int n);
void func_001C1BA0(void *r, X *p);
}
extern float D_002A76D0, D_002A76D4;
static inline void *Alloc(int cb) { return func_001AE318(AREQ(cb, 0x10)); }
static inline qword MulPt(M4 *m, qword v) { qword r; __asm__("vmulax.xyz $ACC, %1, %2x\n\tvmadday.xyz $ACC, %3, %2y\n\tvmaddaz.xyz $ACC, %4, %2z\n\tvmaddw.xyz %0, %5, $vf0w" : "=j"(r) : "j"(m->r[0]), "j"(v), "j"(m->r[1]), "j"(m->r[2]), "j"(m->r[3])); return r; }
static inline M4 *GetMat(X *p) { return p->vt[31].pfn((X *)((char *)p + p->vt[31].d)); }
static inline qword Min(qword a, qword b) { qword r; __asm__("vmini.xyz %0, %1, %2" : "=j"(r) : "j"(a), "j"(b)); return r; }
static inline qword Max(qword a, qword b) { qword r; __asm__("vmax.xyz %0, %1, %2" : "=j"(r) : "j"(a), "j"(b)); return r; }

/** @sly2-annot addr=0x1BD040
 * Purpose: Transform polygon to world, find its pathzone (PpathzoneFromPos), compute bounds,
 *   AddPathzoneObstacle
 * Info: proposed name RegisterCid14Obstacle (T2); class CID14; TU tu_1BC050
 * Notes: names_v2 C proposed UpdateJtStand__FP2JT (0.638)
 * Confidence: low (draft) [T2 0.50] - evidence: static naming pass; sources: r5b_rest
 * See: docs/reference/functions/physics-collision.md#tu_1bc050-0x1bc050-0x1be398
 * @end-sly2-annot */
extern "C" void func_001BD040(X *p)
{
    CTX ctx;
    int i;
    qword *a = (qword *)Alloc((p->c << 4) - 1);
    M4 m = *GetMat(p);
    void *r;
    {
        qword v0 = MulPt(&m, p->apos[0]);
        p->vmin.q = v0;
        p->vmax.q = v0;
        a[0] = v0;
    }
    a[p->c - 1] = p->vmin.q;
    for (i = 1; i < p->c - 1; i++) {
        qword v = MulPt(&m, p->apos[i]);
        p->vmin.q = Min(p->vmin.q, v);
        p->vmax.q = Max(p->vmax.q, v);
        a[i] = v;
    }
    p->vmin.f[2] = D_002A76D0;
    p->vmax.f[2] = D_002A76D4;
    r = func_001C0D70(func_001A62E8(p), 0);
    if (!r)
        return;
    func_001C1BA0(r, p);
}
