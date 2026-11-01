/* func_001E9C88: samples the path po from its start to its end in steps of D_002C367C, collects the distinct
 * objects found at each sample (func_001C0D70) into apv[64] and the parameter ranges where consecutive samples
 * do not stay inside one object (func_001C0920) into aseg[64] (joining the first/last range when po->fE4), then copies
 * both lists to the heap (func_001AE238) into ps.
 * Matching note: the &apv argument of the final func_001AE238 call must go through an inline helper; integrate.c
 * then puts sp+0x200 straight into a1, so gcse has nothing to hoist and the apv[cpv++] cursor (s6) keeps an
 * un-noted init in the loop preheader. The MSG request reuses pos2's stack slot. */
typedef int qword __attribute__((mode(TI)));
struct MSG { int cb; int nKind; };
struct SEG { float a, b; };
struct O { char padC[0xC]; void *vt; char pad10[0xE0-0x10]; int curved; int fE4; int count; float *parameters; float *distances; qword *points; };
struct S { struct O *po; int c4; struct SEG *aseg; int cC; struct SEG aseg10[16]; int c90; void **apv; int n98; };
extern float D_002C367C;
float func_001E6620(struct O *);
float func_001E6648(struct O *);
void func_001E6F10(struct O *, float, qword *, qword *);
void *func_001C0D70(qword, int);
int func_001C0920(void *, qword, int);
void *func_001AE238(struct MSG *, void *);
void func_001AE5A0(void *, const void *, unsigned);
static inline void *PvReallocMsg(struct MSG *pmsg, int cb, int nKind, void *pv) { pmsg->cb = cb; pmsg->nKind = nKind; return func_001AE238(pmsg, pv); }
/** @sly2-annot addr=0x1E9C88
 * Purpose: Sample the path, collect distinct containing objects (0x1C0D70) and parameter ranges
 * Info: proposed name BuildShapeVolumeSegments (T2); TU shape
 * Confidence: low (draft) [T2 0.30] - evidence: static analysis; sources: r6b_rest
 * See: docs/reference/functions/physics-collision.md#shape-0x1e5d80-0x1ea568
 * @end-sly2-annot */
void func_001E9C88(struct S *ps, struct O *po)
{
    struct SEG aseg[64];
    void *apv[64];
    qword pos;
    qword pos2;
    struct SEG *pseg = 0;
    int cpv = 0;
    int cseg = 0;
    float tStart, tPrev, t, tEnd;
    void *pPrev;

    ps->po = po;
    tStart = func_001E6620(po);
    tPrev = tStart;
    t = tStart;
    tEnd = func_001E6648(ps->po);
    func_001E6F10(ps->po, tStart, &pos, 0);
    pPrev = func_001C0D70(pos, 0);
    for (; t < tEnd; t += D_002C367C) {
        void *pCur;
        int outside;
        func_001E6F10(ps->po, t, &pos2, 0);
        pCur = func_001C0D70(pos2, 0);
        if (pCur) {
            int i;
            for (i = 0; i < cpv; i++)
                if (apv[i] == pCur)
                    break;
            if (i >= cpv && (unsigned)cpv < 64)
                apv[cpv++] = pCur;
        }
        outside = 0;
        if (!pCur || pPrev != pCur || !func_001C0920(pPrev, pos2, 0))
            outside = 1;
        if (outside) {
            if (!pseg && cseg < 64) {
                pseg = &aseg[cseg++];
                pseg->a = tPrev;
            }
        } else if (pseg) {
            pseg->b = t;
            pseg = 0;
        }
        tPrev = t;
        pPrev = pCur;
    }
    if (pseg)
        pseg->b = tEnd;
    if (cseg >= 2 && ps->po->fE4 && aseg[0].a == tStart && aseg[cseg - 1].b == tEnd) {
        aseg[0].a = aseg[cseg - 1].a - tEnd;
        aseg[cseg - 1].b = aseg[0].b + tEnd;
    }
    ps->c4 = cseg;
    ps->aseg = PvReallocMsg((struct MSG *)&pos2, cseg * 8, 0x10, aseg);
    ps->cC = cseg;
    func_001AE5A0(ps->aseg10, aseg, cseg * 8);
    ps->c90 = cpv;
    ps->apv = PvReallocMsg((struct MSG *)&pos2, cpv * 4, 0x10, apv);
    ps->n98 = -1;
}
