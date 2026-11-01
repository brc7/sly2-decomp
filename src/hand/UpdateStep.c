struct VT;
struct S {
    char pad0[0xC]; struct VT *vt;
    char pad1[0x180 - 0x10]; float f180;
    char pad2[0x1A4 - 0x184]; int k1A4; float f1A8;
    char pad3[0x208 - 0x1AC]; int n208; float t20C;
};
struct VE { short d; short pad; float *(*pf)(void *); };
struct VT { struct VE e[32]; };
extern struct { float t; float dt; } D_002DDED0;
extern float D_002C3C80[3];
extern float D_002BBB60[2];
void UpdateSo(struct S *p);
void UpdateStepPhys(struct S *p);
float func_002458F0(float y, float x);
float RadNormalize__Ff(float a);

/** @sly2-annot addr=0x203FC0
 * Purpose: STEP vt16: UpdateSo, UpdateStepPhys, then turn-rate speed factor +0x180 (CLQ D_002C3C80 of
 *   |radTarget-heading| clamped 0..1, or 1), stamp +0x20C while grounded
 * Info: proposed name UpdateStep (T1); class STEP slot16; TU step
 * Confidence: high [T1 0.85] - evidence: static analysis; sources: r7b_guard
 * See: docs/engine/player.md#11-key-functions,
 *   docs/reference/functions/gameplay-objects.md#rhythm-0x1520c8-0x1539a8,
 *   docs/reference/functions/guards-ai.md#step-0x203948-0x205e70
 * @end-sly2-annot */
void UpdateStep(struct S *p)
{
    UpdateSo(p);
    UpdateStepPhys(p);
    switch (p->k1A4) {
    case 0: {
        float *a = p->vt->e[31].pf((char *)p + p->vt->e[31].d);
        float *b = p->vt->e[31].pf((char *)p + p->vt->e[31].d);
        float d = RadNormalize__Ff(p->f1A8 - func_002458F0(a[1], b[0]));
        float f;
        d = __builtin_fabsf(d);
        f = D_002C3C80[0] + d * (D_002C3C80[1] + d * D_002C3C80[2]);
        {
            float lo = D_002BBB60[0], hi = D_002BBB60[1];
            if (f < lo)
                f = lo;
            else if (hi < f)
                f = hi;
        }
        p->f180 = f;
        break;
    }
    case -1:
    case 1:
        p->f180 = 1.0f;
        break;
    }
    if (p->n208)
        p->t20C = D_002DDED0.t;
}
