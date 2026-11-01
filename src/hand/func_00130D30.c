struct VTE { short d; short pad; void (*pfn)(); };
struct R { char pad[0x80]; float gMin; float gMax; float *pg; };
struct S { int x0; void *p4; struct R *pr; int xC; int x10; int x14; struct VTE *vt; int x1C; int x20; int x24; struct S *pnext; };
struct Q { char pad[0x74]; float g74; };
struct P { int pad[5]; struct Q *pq; int x18; float g1C; float g20; char pad1[0x34 - 0x24]; struct S *ps; char pad2[0x40 - 0x38];
    float g40; float g44; float g48; float g4C; };
int func_00131968(struct P *p);
void func_001317D8(struct P *p, float *pg, float *pg2);
void func_00130EB0(struct P *p);
int func_0012CEE0(struct S *ps);
void func_00230598(void *p4, struct R *pr);
void func_00230CF0(struct P *p, int n);
/** @sly2-annot addr=0x130D30
 * Purpose: Seek to (SEEK kind, t, svt) then UpdateAsegaIeaCur
 * Info: proposed name SeekAsega (T1); TU asega
 * Notes: names_v2 C proposed HandleAsegaEvents__FP5ASEGAP4ASEGPi (0.438)
 * Confidence: high [T1 0.90] - evidence: static naming pass; sources: r1_116918
 * See: docs/engine/animation.md#7-key-functions,
 *   docs/reference/functions/animation.md#asega-0x1308a8-0x131f70
 * @end-sly2-annot */
void func_00130D30(struct P *p, int k, float g, float s)
{
    float t;
    if (func_00131968(p))
        return;
    p->g44 = s;
    p->g40 = s * p->g48 * p->g4C;
    switch (k) {
    case 0: t = g; break;
    case 1: t = p->g1C + g; break;
    case 2: t = p->pq->g74 + g; break;
    }
    if (t != p->g1C) {
        struct S *ps;
        p->g1C = t;
        func_001317D8(p, &p->g1C, &p->g40);
        func_00130EB0(p);
        for (ps = p->ps; ps; ps = ps->pnext) {
            if (func_0012CEE0(ps)) {
                struct R *pr = ps->pr;
                float v = *pr->pg;
                if (v < pr->gMin || pr->gMax < v)
                    ((void (*)(void *))ps->vt[4].pfn)((char *)ps + ps->vt[4].d);
                func_00230598(ps->p4, ps->pr);
            }
        }
        func_00230CF0(p, 0);
    }
    p->g20 = p->g1C;
}
