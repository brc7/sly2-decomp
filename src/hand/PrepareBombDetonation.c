typedef int qword __attribute__((mode(TI)));
typedef struct { qword r[3]; } M3;
extern qword D_002A2DC0;
struct VTE { short d; short pad; void *pfn; };
struct P {
    int pad[3]; struct VTE *vt; char pad1[0x170 - 0x10]; int x170; int x174; int pad2[2]; qword x180; M3 mat;
    qword x1C0; qword x1D0;
};
M3 *func_001A6318(struct P *);
void BuildOrthonormalMatrixZ(M3 *, qword, qword);
void BuildRotateVectorsMatrix__FP6VECTORT0P7MATRIX3(M3 *, qword, qword);
static __inline__ qword MulV(M3 *pm, qword v)
{
    qword o;
    __asm__("vmulax.xyz $ACC, %1, %4x\n\tvmadday.xyz $ACC, %2, %4y\n\tvmaddz.xyz %0, %3, %4z"
            : "=j"(o) : "j"(pm->r[0]), "j"(pm->r[1]), "j"(pm->r[2]), "j"(v));
    return o;
}
static __inline__ void MulStore(struct P *p, M3 *pm)
{
    M3 t, u;
    M3 *pb = func_001A6318(p);
    u.r[0] = t.r[0] = MulV(pm, pb->r[0]);
    u.r[1] = t.r[1] = MulV(pm, pb->r[1]);
    t.r[2] = u.r[2] = MulV(pm, pb->r[2]);
    p->mat = t;
}
/** @sly2-annot addr=0x13B428
 * Purpose: Once: record detonation pos (+0x180), save v/w, orient by mode (+0x170) using the impact normal
 * Info: proposed name PrepareBombDetonation (T1); TU bomb
 * Notes: names_v2 C proposed CalculateAloTransformAdjust__FP3ALOT0P6VECTORP7MATRIX3T2T2 (0.167) | runtime5a
 *   #14: spent darts +0x180 = rest position, +0x1C0 = flight velocity
 * Confidence: high [T1 0.85] - evidence: runtime (save-state RAM); static naming pass; sources:
 *   r2b_rest+runtime
 * See: docs/engine/gameplay-objects.md#key-functions,
 *   docs/reference/functions/gameplay-objects.md#bomb-0x13a9e8-0x13bdf0
 * @end-sly2-annot */
void PrepareBombDetonation(struct P *p, qword pos, qword q)
{
    M3 m;
    if (p->x174)
        return;
    p->x180 = pos;
    p->x174 = 1;
    {
        struct VTE *e = &p->vt[33];
        p->x1C0 = ((qword (*)(void *))e->pfn)((char *)p + e->d);
    }
    {
        struct VTE *e = &p->vt[34];
        p->x1D0 = ((qword (*)(void *))e->pfn)((char *)p + e->d);
    }
    switch (p->x170) {
    case 0:
        BuildRotateVectorsMatrix__FP6VECTORT0P7MATRIX3(&m, func_001A6318(p)->r[2], q);
        MulStore(p, &m);
        break;
    case 1:
        BuildOrthonormalMatrixZ(&m, p->x1C0, D_002A2DC0);
        p->mat = m;
        break;
    case 2:
        BuildRotateVectorsMatrix__FP6VECTORT0P7MATRIX3(&m, func_001A6318(p)->r[0], q);
        MulStore(p, &m);
        break;
    case 3:
        p->mat = *func_001A6318(p);
        break;
    }
}
