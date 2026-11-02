struct E { float g0; float g4; };
struct S8 { int ib; int x4; struct E *ae; };
struct L { char pad[0x74]; float g74; char pad2[0xC0 - 0x78]; int cb; int c; char *ab; };
struct W { float w; struct L *pl; };
struct V;
struct VT { short d; short p; void (*f)(); };
struct V { struct VT *vt; };
struct P { int x0; struct L *pl; struct S8 *ps; struct V **apv; float g10; };

extern int ImrsgLookUp__FffiP4MRSGPf(int, int, float, float, float *);

/** @sly2-annot addr=0x138BA0
 * Purpose: Blended-channel actuator virtual (p, t, r, pa, pb): weighted sum of two scalars (e.g. twist and
 *   its rate) across bls
 * Info: proposed name EvaluateBlendChnScalar (T2); TU blend
 * Confidence: low (draft) [T2 0.30] - evidence: static analysis; sources: r2b_rest
 * See: docs/reference/functions/animation.md#blend-0x137a30-0x138f08
 * @end-sly2-annot */
void func_00138BA0(struct P *p, float t, float r, float *pa, float *pb)
{
    float g0;
    float g4;
    float g8;
    float rate;
    float sa, sb;
    struct E *pe;
    struct L *pl;
    int i;
    int idx;

    if (p->ps == 0) {
        rate = t / p->pl->g74;
    } else {
        idx = ImrsgLookUp__FffiP4MRSGPf(p->ps->x4, (int)p->ps->ae, t, p->pl->g74, &g0);
        pe = &p->ps->ae[idx];
        rate = g0 / pe->g4;
    }
    sb = 0.0f;
    sa = sb;
    for (i = 0; i < p->pl->c; i++) {
        struct W *pw = (struct W *)(p->pl->ab + i * p->pl->cb);
        struct V *pv;
        if (pw->w == 0.0f)
            continue;
        pv = p->apv[i];
        if (pv == 0) {
            g8 = 0;
            g4 = p->g10;
        } else {
            float u, s;
            if (p->ps == 0) {
                float lim = pw->pl->g74;
                s = r * lim / p->pl->g74;
                u = rate * lim;
            } else {
                struct E *pf = &((struct E *)((char *)pw + p->ps->ib))[idx];
                float lim = pw->pl->g74;
                u = pf->g0 + rate * pf->g4;
                s = r * pf->g4 / pe->g4;
                if (lim < u)
                    u -= lim;
            }
            ((void (*)(void *, float, float, float *, float *))pv->vt[2].f)((char *)pv + pv->vt[2].d, u, s, &g4, &g8);
        }
        sa += pw->w * g4;
        sb += pw->w * g8;
    }
    if (pa)
        *pa = sa;
    if (pb)
        *pb = sb;
}
