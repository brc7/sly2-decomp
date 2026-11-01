typedef int qword __attribute__((mode(TI)));
typedef struct { qword r[3]; qword pos; } M4;
struct VT { short d; short pad; void *f; };
struct O { char pad[0xA0]; void *xA0; };
struct G { char pad[0x13D0]; int k13D0; char pad2[0x1910 - 0x13D4]; int x1910; int k1914; char pad3[4]; struct Q *pq; };
struct Q { char pad[0xC]; struct VT *vt; };
struct P {
    char pad[0xC]; struct VT *vt; char pad1[0x54 - 0x10]; struct O *p54; struct O *p58; struct O *p5C;
    char pad2[0x1A4 - 0x60]; int x1A4; char pad3[0x9E4 - 0x1A8]; int x9E4;
    char pad4[0x1030 - 0x9E8]; int x1030; int x1034; char pad5[0x1800 - 0x1038];
    int k1800; float t1804; void *apv[2]; void *pv1810; void *pv1814;
};
extern struct G *D_002F7900;
struct CLOCK { float t; float dt; };
extern struct CLOCK D_002DDED0;
extern qword D_002A2DD0;
void func_001A58D0(struct P *, int, int);
void func_0012E460(void *);
void func_002303A0(struct P *, struct O *, int);
float func_00204740(struct P *);
void func_00204660(struct P *, float);
void func_00204CA8(struct P *);
void func_002206C8(void *, int);
M4 func_0022FF40(void *, void *, M4 *);
void *func_0012DC18(struct P *p, struct O *pso, int a, int b, int c, qword q);
qword func_0011BFF0(M4 *pm);
void func_001F15B8(void *);
void func_001305B8(void *, struct P *, int, void *, float, float);

/** @sly2-annot addr=0x136530
 * Purpose: State machine on +0x1800 with reparenting/actuator teardown; also called from jtthrow code
 * Info: proposed name SetBentleyHoverState (T2); TU bentley
 * Confidence: low (draft) [T2 0.30] - evidence: static analysis; sources: r2b_rest
 * See: docs/reference/functions/player.md#bentley-0x134648-0x1378d8
 * @end-sly2-annot */
void func_00136530(struct P *p, int k)
{
    void *pv;
    if (k == p->k1800)
        return;
    switch (p->k1800) {
    case 0: {
        int save = p->x9E4;
        func_001A58D0(p, 0, 0xF);
        p->x9E4 = save;
        func_0012E460(&p->pv1810);
        func_0012E460(&p->pv1814);
        func_002303A0(p, p->p58, 2);
        func_002303A0(p, p->p5C, 2);
        break;
    }
    case 1:
        func_00204660(p, func_00204740(p));
        p->x1A4 = 0;
        func_00204CA8(p);
        break;
    }
    p->k1800 = k;
    p->t1804 = D_002DDED0.t;
    pv = p->apv[k];
    switch (k) {
    case -1: {
        struct G *pg = D_002F7900;
        if (pg->k13D0 != 0xF)
            return;
        if (pg->k1914 == 7)
            return;
        func_002206C8(&pg->x1910, 0);
        return;
    }
    case 0: {
        M4 m;
        int n;
        struct Q *pq = D_002F7900->pq;
        n = ((int (*)(void *, M4 *))pq->vt[32].f)((char *)pq + pq->vt[32].d, &m);
        { M4 t = func_0022FF40(p->p54->xA0, p->p58->xA0, &m); m = t; }
        {
            int save = p->x9E4;
            ((void (*)(void *, struct G *, int, int))p->vt[10].f)((char *)p + p->vt[10].d, D_002F7900, n, 3);
            p->x9E4 = save;
        }
        ((void (*)(void *, qword))p->vt[35].f)((char *)p + p->vt[35].d, D_002A2DD0);
        ((void (*)(void *, qword))p->vt[36].f)((char *)p + p->vt[36].d, D_002A2DD0);
        p->pv1810 = func_0012DC18(p, p->p58, 1, 7, 2, m.pos);
        p->pv1814 = func_0012DC18(p, p->p5C, 1, 7, 2, func_0011BFF0(&m));
        break;
    }
    case 1:
        p->x1A4 = -1;
        break;
    }
    func_001F15B8(&p->x1034);
    func_001305B8(pv, p, 9, &p->x1030, 0.0f, 1.0f);
    ((void (*)(void *))p->vt[90].f)((char *)p + p->vt[90].d);
}
