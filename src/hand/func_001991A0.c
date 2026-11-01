typedef int qword __attribute__((mode(TI)));
typedef union { qword q; float f; } U;
typedef struct { qword r[3]; } M3;
typedef struct { short d; short pad; void (*pfn)(void *, int, int, qword, qword, qword); } VT;
struct O { char pad[0xC]; VT *vt; };
struct C8 { int x0; int x4; };
struct P {
    struct O *x0; char pad[0x1C-4]; int x1c; char pad2[0xC8-0x20]; struct C8 *xc8; char pad3[4];
    qword xd0; qword xe0; qword xf0;
};
extern qword D_002A2DB0;
extern qword D_002A2DC0;
qword func_001A62E8(void *p);
qword func_0022FC48(int n, int m, qword q);
void func_0011E2D8(qword a, qword b, qword c, qword *pm);
/** @sly2-annot addr=0x1991A0
 * Purpose: If air kind 0x11 (wall run) build a constraint frame against the wall (GetNormalVectors) and add
 *   it
 * Info: proposed name AddJtWallRunXps (T2); TU air
 * Confidence: low (draft) [T2 0.30] - evidence: static analysis; sources: r4b_rest
 * See: docs/reference/functions/player.md#stepplayer-0x13ff70-0x144638,
 *   docs/reference/functions/player.md#air-0x195950-0x19cbf8
 * @end-sly2-annot */
void func_001991A0(struct P *p)
{
    if (p->x1c == 0x11) {
        U q, r;
        M3 m;
        U s;
        qword neg, v;
        q.q = func_001A62E8(p->x0);
        r.q = func_0022FC48(p->xc8->x0, 0, p->xd0);
        __asm__("qmtc2.ni %1, %0" : "=j"(s.q) : "r"(-1.0f));
        __asm__("vmulx.xyzw %0, %1, %2x" : "=j"(neg) : "j"(p->xe0), "j"(s.q));
        m.r[0] = neg;
        func_0011E2D8(neg, D_002A2DC0, D_002A2DB0, &m.r[1]);
        __asm__("vmulax.xyz $ACC, %2, %1x\n\tvmadday.xyz $ACC, %3, %1y\n\tvmaddz.xyz %0, %4, %1z"
                : "=j"(v) : "j"(p->xf0), "j"(m.r[0]), "j"(m.r[1]), "j"(m.r[2]));
        __asm__("vsub.xyzw %0, %1, %2" : "=j"(r.q) : "j"(r.q), "j"(v));
        p->x0->vt[82].pfn((char *)p->x0 + p->x0->vt[82].d, 7, p->xc8->x4, r.q, D_002A2DB0, q.q);
    }
}
