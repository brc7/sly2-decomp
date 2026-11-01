// func_001856C0 (Claude agent, lane G17, 2026-10-24): MATCH, no register pins, VU helper asms only.
// From laneG16v1 (46). Keys: (1) points loop as `for (n = 0; n < count; n++)` over points[n] (gcc reverses it: bnez),
// with the edge direction Sub hoisted into the edge body and the edge-plane Cross left inside the loop (loop.c moves it
// after the count test) (46 -> 38). (2) Append(LIST*, const qword&) helper storing through `&l->planes[l->count++]`:
// target store order, and edge-loop pass 2 > 95 insns so 2i+i is not hoisted (38 -> 26). (3) one allocation-only
// "j"(a) on Normalize's first asm (cross vs point VF numbering, 26 -> 18). (4) Transform with an untied "=j" output
// and the vector operand last (Transform rows vf4/7/6/5, 18 -> 1). (5) neighbor checks as mesh->neighbors[i].index[k]
// (addu operand order, 1 -> 0 aligned). (6) `for (int i ...)`: i's pseudo number moves the gcse PRE hash order of
// i+1 / 2i, which fixes the 0xD0/0xD4 spill slots (MATCH).
typedef int qword __attribute__((mode(TI)));
typedef union U { qword q; float f[4]; } U;
typedef struct M4 { U r[4]; } M4;
typedef struct VTE { short delta, pad; void *fn; } VTE;
typedef struct I3 { int index[3]; } I3;
typedef struct MESH { char pad0[0xC]; VTE *vt; char pad10[0xD4]; qword *vertices; int count; I3 *triangles; I3 *neighbors; } MESH;
typedef struct LIST { int count; int pad[3]; qword planes[1]; } LIST;
typedef struct PARAMS { float a, b, duration; } PARAMS;
extern "C" {
extern float D_002DDED4, D_002A5688;
extern void func_0011EE08(qword, qword, qword, qword *, float *, float *);
extern float func_0011D018(float, float, float, PARAMS *, float *);
extern void func_001193C0(qword *, qword, float);
static inline qword Transform(const M4 &m, qword p)
{
    qword r;
    __asm__("vmulax.xyz $ACC, %1, %5x\n\tvmadday.xyz $ACC, %2, %5y\n\tvmaddaz.xyz $ACC, %3, %5z\n\tvmaddw.xyz %0, %4, $vf0w"
        : "=j"(r) : "j"(m.r[0].q), "j"(m.r[1].q), "j"(m.r[2].q), "j"(m.r[3].q), "j"(p));
    return r;
}
static inline qword Sub(const qword &a, const qword &b) { qword r; __asm__("vsub.xyzw %0, %1, %2" : "=j"(r) : "j"(a), "j"(b)); return r; }
static inline qword Cross(const qword &a, const qword &b)
{
    qword r;
    __asm__("vopmula.xyz $ACC, %1, %2\n\tvopmsub.xyz %0, %2, %1" : "=j"(r) : "j"(a), "j"(b));
    return r;
}
static inline qword Normalize(const qword &a)
{
    qword d, one, r;
    __asm__("vmul.xyz %0, %2, %2\n\tvaddw.x %1, $vf0, $vf0w\n\tvadday.x $ACC, %0, %0y\n\tvmaddz.x %0, %1, %0z" : "=j"(d), "=&j"(one) : "j"(a), "j"(a)); /* 2nd "j"(a) is allocation-only (unused in the template): raises a's VF priority */
    __asm__("vsqrt $Q, %1x\n\tvwaitq\n\tvaddq.x %0, $vf0, $Q" : "=j"(d) : "j"(d));
    __asm__("vdiv $Q, $vf0w, %2x\n\tvwaitq\n\tvmulq.xyzw %0, %1, $Q" : "=j"(r) : "j"(a), "j"(d));
    return r;
}
static inline float Dot(const qword &a, const qword &b)
{
    qword d, one;
    float f;
    __asm__("vmul.xyz %0, %2, %3\n\tvaddw.x %1, $vf0, $vf0w\n\tvadday.x $ACC, %0, %0y\n\tvmaddz.x %0, %1, %0z" : "=j"(d), "=&j"(one) : "j"(a), "j"(b));
    __asm__("qmfc2.ni %0, %1" : "=r"(f) : "j"(d));
    return f;
}
}
static inline void ClosestPoint(const qword &point, const qword &start, const qword &end, qword *closest, float *dist)
{
    func_0011EE08(point, start, end, closest, 0, dist);
}
static inline void Evaluate(const float &r, const float &d, const float &dt, PARAMS *params, float *out)
{
    func_0011D018(r, d, dt, params, out);
}
static inline void BuildPlane(qword *plane, const qword &normal, const float &g)
{
    func_001193C0(plane, normal, g);
}
static inline U *Addr(U *p) { return p; }
static inline void Append(LIST *l, const qword &q) { qword *p = &l->planes[l->count++]; *p = q; }
/** @sly2-annot addr=0x1856C0
 * Purpose: Gather contact planes of nearby mesh edges/triangles for a set of points within radius
 * Info: proposed name CollectMeshContacts (T2); TU tu_185130
 * Confidence: low (draft) [T2 0.35] - evidence: static analysis; sources: r4b_rest
 * See: docs/reference/functions/player.md#tu_185130-0x185130-0x186610
 * @end-sly2-annot */
extern "C" void func_001856C0(MESH *mesh, LIST *out, int count, qword *points, PARAMS *params, float radius)
{
    M4 mat;
    U v[3];
    U closest;
    U plane;
    float dist, clamped;
    int edge, n;
    M4 *pm;
    VTE *e = &mesh->vt[31];
    float stop, step;
    pm = ((M4 *(*)(void *))e->fn)((char *)mesh + e->delta);
    mat = *pm;
    float h = (params->a + params->b) * 0.5f;
    step = D_002DDED4 * 0.5f;
    stop = params->duration * h;
    for (int i = 0; i < mesh->count; i++) {
        I3 *t = &mesh->triangles[i];
        qword normal;
        if (mesh->neighbors[i].index[0] != -1 && mesh->neighbors[i].index[1] != -1 && mesh->neighbors[i].index[2] != -1) continue;
        v[0].q = Transform(mat, mesh->vertices[t->index[0]]);
        v[1].q = Transform(mat, mesh->vertices[t->index[1]]);
        v[2].q = Transform(mat, mesh->vertices[t->index[2]]);
        normal = Cross(Sub(v[1].q, v[0].q), Sub(v[2].q, v[0].q));
        for (edge = 0; edge < 3; edge++) {
            qword start, end, best_plane;
            float best;
            qword *pp;
            if (((int *)mesh->neighbors)[i * 3 + edge] != -1) continue;
            start = Addr(&v[(edge + 1) % 3])->q;
            end = Addr(&v[(edge + 2) % 3])->q;
            best = D_002A5688;
            for (n = 0; n < count; n++) {
                qword point = points[n];
                ClosestPoint(point, start, end, &closest.q, &dist);
                if (dist < radius + stop) {
                    qword unit = Normalize(Cross(normal, Sub(end, start)));
                    Evaluate(radius, Dot(unit, Sub(point, closest.q)), step, params, &clamped);
                    if (clamped < best) {
                        best = clamped;
                        BuildPlane(&plane.q, unit, best);
                        best_plane = plane.q;
                    }
                }
            }
            if (best != D_002A5688) Append(out, best_plane);
        }
    }
}
