/* Matched by Claude agent (2026-10-17): compare against the parameters directly (no local E copy) and `for (i = 0, pe = p->ae; ...)`. */
struct VTE { short d; short pad; void (*pfn)(void *, ...); };
struct O { int pad[3]; struct VTE *vt; };
struct E { int prio; struct O *po; int a; int b; };
struct P { char pad[0x30C]; int c; struct E ae[8]; char pad2[0x390 - 0x390]; float t; };
struct CLOCK { float t; float dt; };
extern struct CLOCK D_002DDED0;
/** @sly2-annot addr=0x14E4C8
 * Purpose: Remove acpr[] entries matching grf mask (1 cpp, 2 pcplcy, 4 psoFocus, 8 pv), vt5 Revoke each; if
 *   top removed: Deactivate it and Activate new top
 * Info: proposed name RevokeCmPolicy (T1); TU cm
 * Confidence: high [T1 0.90] - evidence: semantic review; sources: r2_131F70
 * See: docs/engine/camera.md#5-key-functions, docs/reference/functions/camera.md#tu_12e600-0x12e600-0x12eb48,
 *   docs/reference/functions/camera.md#cm-0x14d3d8-0x14fac8
 * @end-sly2-annot */
void RevokeCmPolicy(struct P *p, int grf, int prio, struct O *po, int a, int b)
{
    struct E *peOut;
    struct E *pe;
    int i;
    struct O *poTop;
    int aTop;
    int bTop;
    poTop = 0;
    aTop = 0;
    bTop = 0;
    peOut = p->ae;
    for (i = 0, pe = p->ae; i < p->c; i++, pe++) {
        if ((((grf ^ 1) & 1) || prio == pe->prio) &&
            (!(grf & 2) || po == pe->po) &&
            (!(grf & 4) || a == pe->a) &&
            (!(grf & 8) || b == pe->b)) {
            if (i == 0) {
                poTop = pe->po;
                aTop = pe->a;
                bTop = pe->b;
            }
            {
                struct O *o = pe->po;
                struct VTE *v = &o->vt[5];
                ((void (*)(void *, int))v->pfn)((char *)o + v->d, pe->b);
            }
        } else {
            *peOut++ = *pe;
        }
    }
    p->c = peOut - p->ae;
    if (poTop) {
        struct VTE *v = &poTop->vt[3];
        ((void (*)(void *, int, int))v->pfn)((char *)poTop + v->d, aTop, bTop);
    if (p->c > 0) {
        struct O *o;
        struct VTE *v;
        p->t = D_002DDED0.t;
        o = p->ae[0].po;
        v = &o->vt[2];
        ((void (*)(void *, int, int))v->pfn)((char *)o + v->d, p->ae[0].a, p->ae[0].b);
    }
    }
}
