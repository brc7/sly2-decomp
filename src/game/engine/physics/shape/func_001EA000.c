// func_001EA000 (lane 5): matched. Samples an object's path over [tStart, tEnd] and collects the time segments where
// func_001E9F50 finds a valid position into ps->aseg10 (func_001BD560 == 3 means "no bound found", so tStart/tEnd are
// used), intersects them with ps->aseg through func_0014C9F8 and fixes up a wrap-around pair at the end.
// Notes for whoever touches this again:
//  - `pseg` (the aseg10 pointer used by the wrap-around fix-up) must be assigned after the copy loops, outside the block
//    that uses it, so cse does not fold pseg[0] into 0x10(ps); gcse then keeps it as a separate spilled copy.
//  - `nIter` is a dead iteration counter. It only adds a few insns before gcse, which resizes gcse's expression hash
//    table (101 -> 103 buckets) and so renumbers the PRE reaching registers; that decides the order of the two spill
//    slots (0xC1C/0xC20) for &ax[0] and the aseg10 copy. Without it the two slots are swapped.
typedef int qword __attribute__((mode(TI)));
struct VEC { float x, y, z, w; } __attribute__((aligned(16)));
struct SEG { float a, b; };
struct O { char pad[0xE4]; int fE4; };
struct PV { char pad[0x114]; int n114; };
struct S {
    struct O *po;
    int c4;              // number of segments in aseg
    struct SEG *aseg;    // input segments
    int cC;              // number of segments in aseg10 (output)
    struct SEG aseg10[16];
    int c90;             // number of entries in apv
    struct PV **apv;
    int n98;             // stamp of the last update
};
struct X { char pad[0x40]; float a; float b; char pad2[0x18]; } __attribute__((aligned(16)));
struct G { char pad[0x3D70]; int n3D70; };
extern struct G D_003DD490;
extern float D_002C367C;
float SMinShape(struct O *);
float SMaxShape(struct O *);
void EvaluateShapeS(struct O *, float, qword *, qword *);
void *func_001E9F50(qword);
int func_001BD560(void *, struct O *, float, int, qword *, float *);
void *func_002333B4(void *, int, unsigned int);
int func_0014C9F8(int, struct X *, int, struct X *, int, struct X *, int);

/** @sly2-annot addr=0x1EA000
 * Purpose: Sample the path over [t0,t1] and collect valid parameter ranges, intersect with existing ones
 * Info: proposed name BuildShapeValidRanges (T2); TU shape
 * Confidence: low (draft) [T2 0.30] - evidence: static analysis; sources: r6b_rest
 * See: docs/reference/functions/physics-collision.md#shape-0x1e5d80-0x1ea568
 * @end-sly2-annot */
void func_001EA000(struct S *ps)
{
    struct VEC pos;
    struct X ax[16];
    struct X ay[16];
    float segStart, segEnd;
    struct SEG *aseg;
    struct SEG *asegSrc;
    int cseg, c, i, j, k, n, nIter = 0;
    struct SEG *pseg;
    float tStart, t, tEnd;
    {
        int k;
        for (k = 0; k < ps->c90; k++)
            if (ps->n98 < ps->apv[k]->n114)
                break;
        if (k >= ps->c90)
            return;
    }
    aseg = ps->aseg10;
    cseg = 0;
    tStart = SMinShape(ps->po);
    t = tStart;
    tEnd = SMaxShape(ps->po);
    asegSrc = ps->aseg;
    c = ps->c4;
    while (t < tEnd) {
        void *pv;
        EvaluateShapeS(ps->po, t, (qword *)&pos, 0);
        pv = func_001E9F50(*(qword *)&pos);
        if (pv) {
            int fSave = ps->po->fE4;
            struct SEG *pseg;
            int fNew;
            int idx;
            ps->po->fE4 = 0;
            if (func_001BD560(pv, ps->po, t, 0, 0, &segStart) == 3)
                segStart = tStart;
            if (func_001BD560(pv, ps->po, t, 1, 0, &segEnd) == 3)
                segEnd = tEnd;
            ps->po->fE4 = fSave;
            pseg = &aseg[cseg];
            fNew = 1;
            while (aseg < pseg) {
                struct SEG *previous = pseg - 1;
                if (previous->b < segStart)
                    break;
                pseg = previous;
                if (pseg->a < segStart) {
                    fNew = 0;
                    break;
                }
            }
            idx = pseg - aseg;
            cseg = idx + 1;
            if (!((unsigned)cseg < 17))
                cseg = idx;
            if (fNew)
                pseg->a = segStart;
            pseg->b = segEnd;
            t = segEnd;
        }
        nIter++;
        t += D_002C367C;
    }
    for (i = 0; i < c; i++) {
        func_002333B4(&ax[i], 0, 0x60);
        ax[i].a = asegSrc[i].a;
        ax[i].b = asegSrc[i].b;
    }
    for (j = 0; j < cseg; j++) {
        func_002333B4(&ay[j], 0, 0x60);
        ay[j].a = aseg[j].a;
        ay[j].b = aseg[j].b;
    }
    pseg = ps->aseg10;
    n = func_0014C9F8(cseg, ay, c, ax, 0x10, ay, 0);
    for (k = 0; k < n; k++) {
        aseg[k].a = ay[k].a;
        aseg[k].b = ay[k].b;
    }
    if (n >= 2 && ps->po->fE4) {
        // wrap-around: the first segment starts at tStart and the last ends at tEnd, so shift them across the seam
        if (pseg[0].a == tStart && pseg[n - 1].b == tEnd) {
            pseg[0].a = pseg[n - 1].a - tEnd;
            pseg[n - 1].b = pseg[0].b + tEnd;
        }
    }
    ps->cC = n;
    ps->n98 = ++D_003DD490.n3D70;
}
