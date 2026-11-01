/* func_001BDE80: merge a set of convex polygons into one new polygon (hull of all their transformed points), re-centre
   it on the average point, then reparent the old polygons' children to the new one (lane 10).
   X is the polygon object (apos = point array, c = point count incl. the closing duplicate, vt = vtable).

   Notes on the compile (all general tricks, see build/lane_notes/lane10.md):
   - first loop is a plain `for`; the `if(n>0){do..while}` form makes gcse hoist `lui %hi(D_002A2DD0)` to the function entry.
   - the destination pointer `pw` is explicit: loop.c will not strength-reduce `apos + k` by itself (benefit 4 == add cost 4).
   - MulPt uses a plain "=j" output (an in/out "+j" or an earlyclobber changes the VF allocation).
   - px lives in $s1 for both polygon loops in the original; without the pin it swaps with the polygon index (s0/s1)
     (single register pin; GetMat is written out by hand because an inline helper's parameter copy fights the pin). */
typedef int qword __attribute__((mode(TI)));
typedef struct { qword r[4]; } M4;
struct CTX;
extern "C" CTX*func_001AE478(CTX*);
extern "C" void func_001AE4A0(CTX*,int);
struct CTX { CTX(){(void)func_001AE478(this);}~CTX(){func_001AE4A0(this,2);} };
struct AREQ { int cb; int align; AREQ(int c, int a) { cb = c; align = a; } };
struct DL { void *pvFirst; void *pvLast; int ibDle; };
struct DLI { DL *pdl; void **ppv; int ibDle; DLI *pdliNext; };
struct X;
struct VT { short d; short pad; void *pfn; };
struct X { int x0, x4, x8; VT *vt; char pad[0xE4 - 0x10]; int xE4; int c; char pad2[0xF4 - 0xEC]; qword *apos;
           char pad3[0x190 - 0xF8]; int x190; char pad4[0x1C0 - 0x194]; int x1C0; DL dl; char pad5[0x1D8 - 0x1D0]; X*x1D8; };
extern DLI *D_002DD2FC;
extern qword D_002A2DD0;
extern DL D_003E1A40;
extern "C" {
void *func_001AE318(const AREQ &r);
X *func_001BDA00(void);
void func_001BDBF0(X *p, int n, qword *pts);
void func_001BDE18(X *p, X *pNew);
void RemoveDlEntry__FP2DLPv(DL *pdl, void *pv);
void AppendDlEntry__FP2DLPv(DL *pdl, void *pv);
void MeasureShape(X *p);
int func_001BC2F8(X *p);
}
static inline void *Alloc(int cb) { return func_001AE318(AREQ(cb, 0x10)); }
/* point * 4x4 matrix (xyz only, w from row 3) */
static inline qword MulPt(M4*m,qword*point){
 qword out;
 __asm__("vmulax.xyz $ACC,%1,%5x\n\tvmadday.xyz $ACC,%2,%5y\n\tvmaddaz.xyz $ACC,%3,%5z\n\tvmaddw.xyz %0,%4,$vf0w":"=j"(out):"j"(m->r[0]),"j"(m->r[1]),"j"(m->r[2]),"j"(m->r[3]),"j"(*point));return out;
}
static inline void InitDli(DLI *pdli, DL *pdl)
{
    pdli->pdl = pdl;
    pdli->ppv = (void **)pdl;
    pdli->ibDle = pdl->ibDle;
    pdli->pdliNext = D_002DD2FC;
    D_002DD2FC = pdli;
}
static inline void *PvNextDli(DLI *pdli)
{
    void *pv = *pdli->ppv;
    pdli->ppv = (void **)((char *)pv + pdli->ibDle);
    return pv;
}

extern "C" void func_001BDE80(int n, X **ap)
{
    int c = 0;
    int i, j, k;
    qword *apos;
    X *pNew;
    for (int i0 = 0; i0 < n; i0++) c += ap[i0]->c - 1;
    CTX ctx;
    apos = (qword *)Alloc(c << 4);
    register X *px __asm__("$17");   /* the one allowed pin: original keeps px in $s1 (polygon index in $s0) */
    int polygon = 0;
    if (n > 0) {
    k = -16;
    do {
        px = ap[polygon];
        M4 m=*((M4 *(*)(X *))px->vt[31].pfn)((X *)((char *)px + px->vt[31].d));   /* virtual GetMatrix() */
        j = 0;
        if (j < px->c - 1) {
            qword *pw = (qword*)((unsigned)k + (unsigned)apos);
            do { k += 16; *++pw = MulPt(&m,&px->apos[j]); j++; } while (j < px->c - 1);
        }
    } while (++polygon < n);
    }
    pNew = func_001BDA00();
    func_001BDBF0(pNew, c, apos);
    {
        qword sum = D_002A2DD0;
        qword k2;
        union { qword q; float f[4]; } u;   /* the scalar's stack home (dead store in the original) */
        for(int sumIndex=pNew->c-2;sumIndex>=0;--sumIndex)
            __asm__("vadd.xyzw %0,%0,%1":"+j"(sum):"j"(pNew->apos[sumIndex]));
        __asm__("qmtc2.ni %1, %0" : "=j"(k2) : "r"((float)(pNew->c - 1)));
        /* sum /= (c-1) via vdiv/vmulq; the extra "+j"(k2) is allocation/ordering only: it keeps the dead store of k2 after the vmulq */
        __asm__("vdiv $Q, $vf0w, %2x\n\tvwaitq\n\tvmulq.xyzw %0, %0, $Q" : "+j"(sum), "+j"(k2) : "j"(k2));
        void (*pfn)(X *, qword) = (void (*)(X *, qword))pNew->vt[29].pfn;   /* virtual SetCenter(sum); pfn load must precede the u store */
        pfn((X *)((char *)pNew + pNew->vt[29].d), (u.q = k2, sum));
        int pointIndex = pNew->c;
        while (--pointIndex >= 0) {
            qword point=pNew->apos[pointIndex];
            __asm__("vsub.xyzw %0,%0,%1":"+j"(point):"j"(sum));
            pNew->apos[pointIndex]=point;
        }
    }
    for (i = 0; i < n; i++) {
        px = ap[i];
        if (px->x1C0) {
            DLI dli;
            X *pv;
            InitDli(&dli, &px->dl);
            while ((pv = (X *)PvNextDli(&dli)) != 0) {
                RemoveDlEntry__FP2DLPv(&px->dl, pv);
                pv->x1D8 = 0;
                func_001BDE18(pv, pNew);
            }
            ((void (*)(X *))px->vt[9].pfn)((X *)((char *)px + px->vt[9].d));
            AppendDlEntry__FP2DLPv(&D_003E1A40, px);
            D_002DD2FC = dli.pdliNext;
        } else {
            func_001BDE18(px, pNew);
        }
    }
    pNew->xE4 = 1;
    MeasureShape(pNew);
    pNew->x190 = func_001BC2F8(pNew);
}
