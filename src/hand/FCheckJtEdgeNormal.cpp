// Collision sweep: builds a MATRIX4 from the incoming basis row/point, asks func_001995D8 for 22 sample points
// (11 segments) and tests every segment against the shapes of every layer; returns whether at least 2 hits
// had a plane normal with Z > 0.7 (any hit before the 3rd segment, or with parameter 0, fails at once).
//
// Tricks that mattered (lane 8):
//  * the segment test is a small inline helper taking POINTERS to the two end points: its pointer parameters become
//    pseudos copied at the call site, which gives the target's "cursor copy in $s6 + recomputed &apt[i+1]" pair
//    without any strength-reduced second pointer;
//  * the loop bound is `sizeof apt / sizeof *apt` (size_t compare -> sltiu) while `i < 4` stays an int compare;
//  * m4 is touched both through a pointer (pm: row copy, pos.w) and by name (zeroing of the three w lanes); that
//    mix is what keeps rows 1,2 s0-relative and the w stores $sp-relative like the original.
typedef int qword __attribute__((mode(TI)));
union U { qword q; float f[4]; int i[4]; };
struct M3 { U r[3]; };
struct M4 { M3 m; U pos; };
struct PLANE { qword q; };
// HIT is cleared 0x60 bytes at a time by the collision routines; the original frame reserves 0x70 for it.
struct HIT { U point[2]; U plane[2]; float parameter[2]; int kind; PLANE *face[2]; int unk54[5]; };
struct BSP;
struct S0 { char pad[0x10]; long x10; char pad2[0x60 - 0x18]; S0 *x60; char pad64[0x10]; BSP *tree; };
struct EC2 { char pad[0x34]; struct S0 *x34; char pad2[0x70 - 0x38]; long x70; };
struct C { char pad[0xEC]; struct EC2 *xec; };
struct L { int x0; struct L *x4; int x8; struct C *xc; };
struct EC { char pad[0x64]; struct L *x64; };
struct O { char pad[0xEC]; struct EC *xec; };
struct P { struct O *x0; };
extern qword D_002A2DC0;
extern qword D_002A2DB0;
extern "C" {
void GetNormalVectors(qword a, qword b, qword c, qword *pm);
qword *func_00256D18(qword *pq, qword q);
void func_001995D8(struct P *p, const M4 *pm, int n, U *apt);
int func_0014C6A0(struct S0 *s, qword a, qword b, int n, HIT *ph);
}

static inline int HitSegment(struct S0 *s, const qword *a, const qword *b, HIT *h)
{
    return func_0014C6A0(s, *a, *b, 1, h);
}

extern "C" int FCheckJtEdgeNormal(struct P *p, qword q1, qword q2)
{
    M3 m;
    M4 m4;
    qword apt[22];
    HIT hit;
    struct L *l;
    int c = 0;
    m.r[0].q = q2;
    GetNormalVectors(q2, D_002A2DC0, D_002A2DB0, &m.r[1].q);
    M4 *pm = &m4;
    (void)func_00256D18(&m4.pos.q, q1);
    pm->m = m;
    m4.m.r[0].i[3] = m4.m.r[1].i[3] = m4.m.r[2].i[3] = 0;
    pm->pos.f[3] = 1.0f;
    func_001995D8(p, &m4, 22, (U *)apt);
    for (l = p->x0->xec->x64; l; l = l->x4) {
        int i;
        if (l->xc->xec->x70 & 4)
            continue;
        for (i = 0; i < sizeof apt / sizeof *apt; i += 2) {
            struct S0 *s;
            for (s = l->xc->xec->x34; s; s = s->x60) {
                if (s->x10 & (1L << 48))
                    continue;
                if (HitSegment(s, &apt[i], &apt[i + 1], &hit) <= 0)
                    continue;
                if (i < 4)
                    return 0;
                if (hit.parameter[0] == 0.0f)
                    return 0;
                if (hit.plane[0].f[2] > 0.7f)
                    c++;
            }
        }
    }
    return c >= 2;
}

typedef char guard_S0_x10[(unsigned)&((S0 *)0)->x10 == 16 ? 1 : -1];
typedef char guard_S0_x60[(unsigned)&((S0 *)0)->x60 == 96 ? 1 : -1];
typedef char guard_EC2_x34[(unsigned)&((EC2 *)0)->x34 == 52 ? 1 : -1];
typedef char guard_EC2_x70[(unsigned)&((EC2 *)0)->x70 == 112 ? 1 : -1];
typedef char guard_M4_pos[(unsigned)&((M4 *)0)->pos == 48 ? 1 : -1];
typedef char guard_HIT_param[(unsigned)&((HIT *)0)->parameter == 64 ? 1 : -1];
typedef char guard_size_HIT[sizeof(HIT) == 112 ? 1 : -1];
