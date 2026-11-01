// MATCH (Claude agent, laneC2c 2026-10-23): v10 plus b copied through an empty volatile "+j" asm before the 2nd Madd (the b reload is issued before the d load).
typedef int qword __attribute__((mode(TI)));
typedef union { qword q; float f[4]; } VU;
struct VTE { short d; short pad; void *pfn; };
struct O { struct VTE *vt; };
struct SEG { float g0; float g4; };
struct CL { char pad[0x74]; float x74; };
struct E { float w; struct CL *p4; };
struct A { char pad[0x74]; float x74; char pad2[0xC0 - 0x78]; int xC0; int xC4; char *xC8; char pad3[0xD4 - 0xCC]; int xD4; };
struct B { int x0; void *x4; struct SEG *x8; };
struct P { int pad; struct A *p4; struct B *x8; struct O **xC; qword x10; };
struct Q {
    char pad[0x80];
    float x80;
    float x84;
    float *x88;
    float *x8C;
    char pad90[0x9A - 0x90];
    short x9A;
    char pad9C[0xC0 - 0x9C];
    VU xC0;
    VU xD0;
    VU xE0;
    VU xF0;
};
int func_00138370(void *, struct SEG *, float *, float, float);
void *func_002333B4(void *, int, int);
void EvaluateBezierPos(float, float, float, qword, qword, qword, qword, VU *, VU *, int);

static __inline__ qword Madd(qword a, qword v, qword s)
{
    qword r;
    __asm__ __volatile__("vadda.xyzw $ACC, %0, $vf0" : : "j"(a), "j"(s) : "memory"); /* unused "j"(s): allocation-only operand (Claude agent) */
    __asm__ __volatile__("vmaddx.xyzw %0, %1, %2x" : "=j"(r) : "j"(v), "j"(s), "0"(a)); /* "0"(a): r shares a's VF reg like the target (Claude agent) */
    return r;
}


static __inline__ qword MaddN(qword a, qword v, qword s)
{
    qword r;
    __asm__ __volatile__("vadda.xyzw $ACC, %0, $vf0" : : "j"(a), "j"(s) : "memory");
    __asm__("vmaddx.xyzw %0, %1, %2x" : "=j"(r) : "j"(v), "j"(s), "0"(a));
    return r;
}

void func_00138660(struct P *p, struct Q *pq)
{
    VU c, d;
    struct Q q;
    VU w;
    float t[2];
    qword a, b;
    float u, ratio;
    int iSeg, i;
    struct A *pa = p->p4;

    if (pa->xD4 >= 0) {
        struct O *po = p->xC[pa->xD4];
        if (po) {
            struct VTE *e = &po->vt[2];
            ((void (*)(void *, struct Q *))e->pfn)((char *)po + e->d, pq);
        } else {
            qword v = p->x10;
            float *pv = pq->xD0.f;
            float *pw = pq->xF0.f;
            float *p80;
            pq->x9A = 2;
            pq->xE0.q = v;
            pq->xC0.q = v;
            pv[2] = 0;
            pv[1] = 0;
            pv[0] = 0;
            pw[2] = 0;
            pw[1] = 0;
            p80 = &pq->x80;
            pq->x8C = &pq->x84;
            pq->x84 = 1.0f;
            pq->x88 = p80;
            pq->x80 = 0.0f;
            pw[0] = 0;
        }
        return;
    }
    {
        float tCur = *pq->x88;
        if (!p->x8) {
            u = tCur / pa->x74;
        } else {
            iSeg = func_00138370(p->x8->x4, p->x8->x8, &t[0], tCur, pa->x74);
            u = t[0] / p->x8->x8[iSeg].g4;
        }
    }
    __asm__("vsub.xyzw %0, $vf0, $vf0" : "=j"(a));
    i = 0;
    b = a;
    for (; i < p->p4->xC4; i++) {
        struct E *pe = (struct E *)(p->p4->xC8 + i * p->p4->xC0);
        struct O *po;
        if (pe->w == 0.0f)
            continue;
        po = p->xC[i];
        if (!po) {
            c.q = p->x10;
            d.f[2] = 0.0f;
            d.f[1] = 0.0f;
            d.f[0] = 0.0f;
        } else {
            struct B *pb = p->x8;
            if (!pb) {
                float g = pe->p4->x74;
                ratio = g / ((struct P volatile *)p)->p4->x74; /* volatile read (Claude agent): the target reloads p->p4 here (no cse with the loop test's load) */
                t[1] = u * g;
            } else {
                struct SEG *ps = (struct SEG *)((char *)pe + pb->x0) + iSeg;
                float gMax = pe->p4->x74;
                float g4 = ps->g4;
                t[1] = ps->g0 + u * g4;
                if (gMax < t[1])
                    t[1] -= gMax;
                ratio = g4 / pb->x8[iSeg].g4;
            }
            func_002333B4(&q, 0, 0x100);
            q.x88 = &t[1];
            {
                struct VTE *e = &po->vt[2];
                ((void (*)(void *, struct Q *))e->pfn)((char *)po + e->d, &q);
            }
            EvaluateBezierPos(q.x84 - q.x80, t[1] - q.x80, ratio, q.xC0.q, q.xD0.q, q.xE0.q, q.xF0.q, &c, &d, 0);
        }
        {
            qword s;
            register int wb asm("$19") = *(int *)&pe->w; /* register pin (Claude agent): target loads pe->w into pe's dying register */
            __asm__ __volatile__("qmtc2.ni %1, %0" : "=j"(s) : "r"(wb));
            a = MaddN(a, c.q, s);
            w.q = s;
            {
            qword bb = b;
            __asm__ __volatile__("" : "+j"(bb)); /* empty asm (Claude agent): b is reloaded here, before the d load, like the target */
            b = Madd(bb, d.q, s);
            }
        }
    }
    pq->x9A = 4;
    pq->xC0.q = a;
    pq->xD0.q = b;
    __asm__ __volatile__("" : : "r"(pq)); /* empty asm (Claude agent): one more use of pq for global-alloc priority (pq s2, pe s3 like the target) */
}
