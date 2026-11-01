// CposFindPath: build a route between two points over the navigation graph (wave-3 lane 4, from w2lane6.v1).
// Finds the triangles containing both points, projects the points onto them (ClosestPointOnCgt), links a temporary start
// vertex to the start triangle's corners and the goal vertex to the goal triangle's corners, runs the path search
// (FindPathAStar) and the path simplifier (SimplifyApcgvNeighbor), copies the route out, drops points closer than the minimum
// spacing, and returns a two-point route (offset along +X) when only one point is left.
// Keys of the last fixes (tail block):
//  * D_002C0C04 is a `const float` read by value through VU_FLOAT(const float) whose qmtc2 input has a never-chosen "V"
//    alternative: the inliner substitutes the global's unchanging MEM for the const parameter, so the word load is an
//    input reload (lui v1 / lw v1) and the read has no anti-dependence on the out[1] store. That drops the qmtc2 asm's
//    sched1 priority below %hi(D_002A2DA0), which gives the axis address v0 in local-alloc, as in the original.
typedef int qword __attribute__((mode(TI)));
union U { qword q; float f[4]; };
struct V;
// volatile cost: stops dbr from filling the loop branch delay slots with the cost store (as in the target)
struct E { V *pv; volatile int cost; };
struct V { U pos; int cEdge; E *aEdge; };
struct TRI { V *apv[3]; };
struct BSP;
struct G { int pad0[3]; TRI *atri; int cv; BSP *pbsp; };
struct AREQ { int cb, align; AREQ() {} AREQ *Set(int n) { cb = n; align = 16; return this; } };
extern const float D_002C0C04; // minimum spacing between route points (50.0f)
extern qword D_002A2DA0;       // (1, 0, 0, 0)
extern "C" {
TRI *PcgtPointInCbspSafe(G *, BSP *, qword, int);
qword ClosestPointOnCgt(TRI *, qword);
void func_001AE478(void *);
void func_001AE4A0(void *, int);
void *func_001AE318(AREQ *);
void FindPathAStar(G *, V *, V *, int, int *, V **);
void SimplifyApcgvNeighbor(G *, int *, V **);
int CposFindPath(G *, qword, qword, int, U *);
}
struct MARK { MARK() { func_001AE478(this); } ~MARK() { func_001AE4A0(this, 2); } };
// the unused second output keeps gcse from hoisting the qmtc2 out of the loops (not single_set)
static inline qword ToVF(qword v) { qword r; int dummy; __asm__("qmtc2.ni %2,%0" : "=j"(r), "=r"(dummy) : "r"(v)); return r; }
static inline int Dist2DI(qword a, qword b)
{
    qword d; int r;
    __asm__("vsub.xyzw %0,%1,%2" : "=j"(d) : "j"(a), "j"(b));
    __asm__("vmul.xy %0,%1,%1\n\tvaddy.x %0,%0,%0y" : "=j"(d) : "j"(d));
    __asm__("vsqrt Q,%1x\n\tvwaitq\n\tvaddq.x %0,$vf0,Q" : "=j"(d) : "j"(d));
    __asm__("qmfc2.ni %0,%1" : "=r"(r) : "j"(d));
    return r;
}
static inline float Dist2D(qword a, qword b)
{
    qword d; float r;
    __asm__("vsub.xyzw %0,%1,%2" : "=j"(d) : "j"(a), "j"(b));
    __asm__("vmul.xy %0,%1,%1\n\tvaddy.x %0,%0,%0y" : "=j"(d) : "j"(d));
    __asm__("vsqrt Q,%1x\n\tvwaitq\n\tvaddq.x %0,$vf0,Q" : "=j"(d) : "j"(d));
    __asm__("qmfc2.ni %0,%1" : "=r"(r) : "j"(d));
    return r;
}
struct VU_FLOAT {
    qword q;
    // never-chosen "V" alternative: allocation-only (the word stays a MEM until reload)
    VU_FLOAT(const float f) { __asm__("qmtc2.ni %1, %0" : "=j,j"(q) : "r,V"(f)); }
};
static inline qword AddScaled(qword p, qword axis, VU_FLOAT s)
{
    qword r;
    __asm__("vadda.xyzw ACC,%1,$vf0\n\tvmaddx.xyzw %0,%2,%3x" : "=j"(r) : "j"(p), "j"(axis), "j"(s.q));
    return r;
}

int CposFindPath(G *g, qword from, qword to, int capacity, U *out)
{
    TRI *first = PcgtPointInCbspSafe(g, g->pbsp, from, 1);
    if (!first) return 0;
    qword start = ClosestPointOnCgt(first, from);
    TRI *last = PcgtPointInCbspSafe(g, g->pbsp, to, 1);
    if (!last) return 0;
    qword finish = ClosestPointOnCgt(last, to);
    U *write;
    int count;
    if (first == last) {
        out[0].q = from;
        out[1].q = start;
        count = 3;
        write = out + 1;
        out[2].q = finish;
    } else {
        MARK mark;
        write = out + 1;
        V **path = (V **)func_001AE318(AREQ().Set(0x200));
        V initial;
        initial.pos.q = start;
        initial.cEdge = 3;
        initial.aEdge = (E *)func_001AE318(AREQ().Set(0x18));
        qword sv = ToVF(start);
        for (int i = 0; i < 3; i++) {
            V *v = first->apv[i];
            E *e = &initial.aEdge[i];
            e->pv = v;
            register int c asm("$4"); // the single pin: the original keeps this qmfc2 result in a0
            c = Dist2DI(sv, v->pos.q);
            e->cost = c;
        }
        V goal;
        goal.pos.q = finish;
        goal.cEdge = 0;
        goal.aEdge = 0;
        qword fv = ToVF(finish);
        V *pg = &goal;
        for (int i = 0; i < 3; i++) {
            V *v = last->apv[i];
            int n = v->cEdge;
            v->cEdge = n + 1;
            E *e = v->aEdge + n;
            e->pv = pg;
            e->cost = Dist2DI(v->pos.q, fv);
        }
        int found;
        FindPathAStar(g, &initial, pg, 128, &found, path);
        if (found >= 3) SimplifyApcgvNeighbor(g, &found, path);
        int n = found; if (n > capacity) n = capacity; count = n;
        for (int i = 0; i < count; i++) out[i].q = path[i]->pos.q;
        for (int i = 0; i < 3; i++) last->apv[i]->cEdge--;
    }
    {
        U *end = out + count;
        qword prev = out[0].q;
        U *rd = write;
        U *wr = write;
        if (wr < end) {
            float thr = D_002C0C04;
            do {
                int accept = thr < Dist2D(rd->q, prev);
                if (accept) {
                    U *s = rd++;
                    U *d = wr++;
                    prev = s->q;
                    d->q = prev;
                } else rd++;
            } while (rd < end);
        }
        count = wr - out;
    }
    if (count == 1) {
        qword p = out[0].q;
        VU_FLOAT s(D_002C0C04);
        out[1].q = p;
        count = 2;
        out[0].q = AddScaled(p, D_002A2DA0, s);
    }
    return count;
}
