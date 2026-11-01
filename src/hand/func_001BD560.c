typedef int qword __attribute__((mode(TI)));
typedef struct HIT { qword q; char pad[0x50]; } HIT;
typedef struct S { char pad[0xE4]; int fWrap; } S;
extern float D_002C0AC0;
void EvaluateShapeS(S *p, qword *pv, int z, float t);
float SWrapShape(S *p, float t);
float SMinShape(S *p);
float SMaxShape(S *p);
int ClsgClipEdgeToCnvo(void *p, qword a, qword b, int c, HIT *ahit);


int func_001BD560(void *p, S *s, int fForward, qword *pq, float *pt, float t)
{
    qword av[2];
    HIT ahit[2];
    float dt;
    int fWrapped;
    int c;

    EvaluateShapeS(s, &av[1], 0, t);
    dt = D_002C0AC0;
    if (!fForward) dt = -dt;
    fWrapped = 0;
    for (;;) {
        av[0] = av[1];
        t += dt;
        if (t <= SMinShape(s)) {
            if (fWrapped) break;
            fWrapped = 1;
            if (s->fWrap) t = SWrapShape(s, t);
            else t = SMinShape(s);
        } else if (SMaxShape(s) <= t) {
            if (fWrapped) break;
            fWrapped = 1;
            if (s->fWrap) t = SWrapShape(s, t);
            else t = SMaxShape(s);
        }
        EvaluateShapeS(s, &av[1], 0, t);
        c = ClsgClipEdgeToCnvo(p, av[0], av[1], 2, ahit);
        if (c > 0) {
            if (pt) *pt = t;
            if (pq) *pq = (ahit - 1)[c].q;
            return -1;
        }
    }
    return 3;
}
