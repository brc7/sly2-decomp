typedef int qword __attribute__((mode(TI)));
typedef union { qword q; float f; } U;
typedef struct { qword r[4]; } M4;
struct P;
typedef struct { short d; short pad; void (*pfn)(void *, struct P *, U *, int, float); } VT1;
typedef struct { short d; short pad; void *(*pfn)(void *, int); } VT2;
struct S { char pad[0x20]; VT1 *vt; };
struct O { char pad[0xC]; VT2 *vt; };
struct R { void *x0; };
struct Q { struct O *x0; struct S *x4; struct R *x8; int xc; U x10; };
struct P { char pad[0x240]; qword x240; };
qword func_0022FC48(void *a, void *b, qword q);
void func_002309E8(void *o, float g, int n, M4 *pm, int m);
/** @sly2-annot addr=0x19F3F8
 * Purpose: World position of a recorded place (object-local pos via ConvertXfPos, or stored world pos)
 * Info: proposed name PosFromJtPlace (T2); TU places
 * Confidence: low (draft) [T2 0.45] - evidence: static analysis; sources: r4b_rest
 * See: docs/reference/functions/player.md#places-0x19f3e8-0x1a0300
 * @end-sly2-annot */
qword func_0019F3F8(struct P *p, struct Q *q, float g)
{
    U t;
    M4 m;
    void *o;
    qword v;
    qword r;
    if (q->x4) {
        struct S *s = q->x4;
        s->vt[3].pfn((char *)s + s->vt[3].d, p, &t, 0, q->x10.f);
        o = q->x0->vt[32].pfn((char *)q->x0 + q->x0->vt[32].d, 0);
        v = func_0022FC48(0, o, t.q);
    } else {
        o = q->x8 ? q->x8->x0 : 0;
        v = q->x10.q;
    }
    if (o && 0.0f < g) {
        func_002309E8(o, g, 0, &m, 0);
        __asm__("vmulax.xyz $ACC, %1, %5x\n\tvmadday.xyz $ACC, %2, %5y\n\tvmaddaz.xyz $ACC, %3, %5z\n\tvmaddw.xyz %0, %4, $vf0w"
                : "=j"(r) : "j"(m.r[0]), "j"(m.r[1]), "j"(m.r[2]), "j"(m.r[3]), "j"(v));
    } else {
        r = func_0022FC48(o, 0, v);
    }
    if (!q->x4)
        __asm__("vsub.xyzw %0, %1, %2" : "=j"(r) : "j"(r), "j"(p->x240));
    return r;
}
