typedef int qword __attribute__((mode(TI)));
typedef struct HIT { qword q; char pad[0x50]; } HIT;
typedef struct S { char pad[0xE4]; int fWrap; } S;
extern float D_002C0AC0;
void func_001E6F10(S *p, qword *pv, int z, float t);
float func_001E66C8(S *p, float t);
float func_001E6620(S *p);
float func_001E6648(S *p);
int func_001BC688(void *p, qword a, qword b, int c, HIT *ahit);


/** @sly2-annot addr=0x1BD560
 * Purpose: Step along a shape until it leaves obstacles
 * Info: proposed name FindShapeFreeRange (T2); TU tu_1BC050
 * Notes: names_v2 B proposed AddJtCustomXps__FP2JTP2SOiP3BSPT3PP2XP (0.856)
 * Confidence: low (draft) [T2 0.30] - evidence: static naming pass; sources: r5b_rest
 * See: docs/reference/functions/physics-collision.md#tu_1bc050-0x1bc050-0x1be398
 * @end-sly2-annot */
int func_001BD560(void *p, S *s, int fForward, qword *pq, float *pt, float t)
{
    qword av[2];
    HIT ahit[2];
    float dt;
    int fWrapped;
    int c;

    func_001E6F10(s, &av[1], 0, t);
    dt = D_002C0AC0;
    if (!fForward) dt = -dt;
    fWrapped = 0;
    for (;;) {
        av[0] = av[1];
        t += dt;
        if (t <= func_001E6620(s)) {
            if (fWrapped) break;
            fWrapped = 1;
            if (s->fWrap) t = func_001E66C8(s, t);
            else t = func_001E6620(s);
        } else if (func_001E6648(s) <= t) {
            if (fWrapped) break;
            fWrapped = 1;
            if (s->fWrap) t = func_001E66C8(s, t);
            else t = func_001E6648(s);
        }
        func_001E6F10(s, &av[1], 0, t);
        c = func_001BC688(p, av[0], av[1], 2, ahit);
        if (c > 0) {
            if (pt) *pt = t;
            if (pq) *pq = (ahit - 1)[c].q;
            return -1;
        }
    }
    return 3;
}
