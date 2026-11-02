// NEAR MISS (Claude agent): 4/150. Hard-reg VF vars in Dot fixed the first block; remaining: 2nd block vsub scheduled before the lui of D_002BD2F0 (target lui first).
typedef int qword __attribute__((mode(TI)));
struct Q { char pad[0x18]; float x18; float x1C; char pad2[0x30 - 0x20]; int x30; };
struct O;
struct VT { char pad[0x2C8]; short d; short pad2; int (*pfn)(struct O *); };
struct O { char pad[0xC]; struct VT *pvt; char pad2[0x30 - 0x10]; int x30; char pad3[0x7C - 0x34]; struct O *x7C; };
struct P {
    char pad[0x10]; void *p10; char pad1[0x4C - 0x14]; int x4C; float x50; char pad2[0x80 - 0x54];
    qword q80; char pad3[0xC0 - 0x90]; struct O *xC0; int xC4; float xC8; int xCC; float xD0;
};
struct CLOCK { float t; };
extern struct CLOCK D_002DDED0;
extern qword D_002A2DC0;
extern float D_002BD2E0[3];
extern float D_002BBB60[2];
extern float D_002BD2F0;
void func_001EF0C0(struct P *p, struct Q *q);
qword func_001A62E8(void *p);
float atan2f(float y, float x);
int FIsBasicDerivedFrom__FP5BASIC3CID(struct O *p, int k);
void UpdatePoctlPressed(struct Q *q);
static __inline__ float GetX(qword q) { float x; __asm__("mtc1 %1,%0" : "=f"(x) : "r"(q)); return x; }
static __inline__ float GetY(qword q) { float y; __asm__("prot3w %0,%1" : "=r"(y) : "r"(q)); return y; }

static __inline__ float LenXY(qword d)
{
    float g;
    qword t;
    __asm__("vmul.xy %0, %1, %1\n\tvaddy.x %0, %0, %0y" : "=j"(t) : "j"(d));
    d = t;
    __asm__("vsqrt Q, %0x\n\tvwaitq\n\tvaddq.x %0, $vf0, Q" : "+j"(d));
    __asm__("qmfc2.ni %0, %1" : "=r"(g) : "j"(d));
    return g;
}
static __inline__ float LenXY2(qword d)
{
    float g;
    qword t;
    /* volatile: raises the priority of the preceding limit load so lui/lwc1 interleave with vsub (Claude agent) */
    __asm__ __volatile__("vmul.xy %0, %1, %1" : "=j"(t) : "j"(d));
    __asm__("vaddy.x %0, %0, %0y" : "+j"(t));
    d = t;
    __asm__("vsqrt Q, %0x\n\tvwaitq\n\tvaddq.x %0, $vf0, Q" : "+j"(d));
    __asm__("qmfc2.ni %0, %1" : "=r"(g) : "j"(d));
    return g;
}

static __inline__ float GClampR(float g, float lo, float hi) { if (g < lo) return lo; if (hi < g) return hi; return g; }
static __inline__ int FFar(qword a, qword b) { qword d; float lim = D_002BD2F0; __asm__("vsub.xyzw %0, %1, %2" : "=j"(d) : "j"(a), "j"(b)); return lim < LenXY2(d); }
static __inline__ qword Dot(qword a, qword b) { register qword r __asm__("vf2"); register qword t __asm__("vf4");
        __asm__("vmul.xyz %1, %2, %3\n\tvaddw.x %0, $vf0, $vf0w\n\tvadday.x $ACC, %1, %1y\n\tvmaddz.x %1, %0, %1z"
                : "=&j"(t), "=&j"(r) : "j"(a), "j"(b)); return r; }
static __inline__ qword Perp(qword d, qword axis) { qword v, r;
        v = Dot(d, axis);
        __asm__("vmulx.xyzw %0, %1, %0x" : "+j"(v) : "j"(axis));
        __asm__("vsub.xyzw %0, %1, %0" : "+j"(v) : "j"(d)); return v; }
static __inline__ float GPoly(float *a, float g) { return a[0] + g * (a[1] + g * a[2]); }
/** @sly2-annot addr=0x144BC8
 * Purpose: BHV168 slot 14: base fills the drive record (0x1EF0C0), then while in states 0/1 sets speed (CLQ
 *   of xy distance, scaled by +0xD0) and heading toward the goal
 * Info: proposed name UpdateBhvwalkgoalPoctl (T1); class BHVWALKGOAL slot14; TU tu_144B28
 * Notes: v3 BHV class word (runtime6 P2: BHV194 = follow, BHV168 = walk-to-goal child): Bhv168_VUnk14 ->
 *   UpdateBhvwalkgoalPoctl | runtime6 P1: slot 14 called by UpdateStepplayer 0x1403D0 with the POCTL; writes
 *   POCTL +0x18 heading = atan2 to goal, +0x1C speed 0 when arrived
 * Confidence: mid (draft) [T1 0.65] - evidence: runtime (save-state RAM); sources: r2b_rest+runtime(class
 *   role)+runtime
 * See: docs/reference/functions/player.md#tu_144b28-0x144b28-0x1471c0
 * @end-sly2-annot */
void UpdateBhvwalkgoalPoctl(struct P *p, struct Q *q)
{
    func_001EF0C0(p, q);
    if (p->x4C < 2) if (p->x4C >= 0) {
        qword pos;
        qword d, v, t, axis;
        float g, s;
        __asm__("lqc2 %0, %1" : "=j"(pos) : "m"(p->q80));
        v = func_001A62E8(p->p10);
        __asm__("vsub.xyzw %0, %1, %2" : "=j"(d) : "j"(pos), "j"(v));
        axis = D_002A2DC0;
        v = Perp(d, axis);
        g = GPoly(D_002BD2E0, LenXY(v));
        s = GClampR(g, D_002BBB60[0], D_002BBB60[1]);
        if (0.0f < s) {
            q->x1C = s * p->xD0;
            q->x18 = atan2f(GetY(v), GetX(v));
        }
        if (p->xCC)
            q->x30 |= 8;
        if (p->xC0 && p->xC8 < D_002DDED0.t - p->x50) {
            int f = 1;
            struct O *po = p->xC0->x7C;
            qword a, b;
            __asm__("qmtc2.ni %1, %0" : "=j"(a) : "r"(func_001A62E8(p->p10)));
            __asm__("qmtc2.ni %1, %0" : "=j"(b) : "r"(func_001A62E8(po)));
            if (FFar(a, b))
                q->x30 |= 8;
            if (FIsBasicDerivedFrom__FP5BASIC3CID(po, 0x34))
                f = po->pvt->pfn((struct O *)((char *)po + po->pvt->d)) != 0;
            if (f) {
                q->x30 |= p->xC0->x30 & p->xC4;
                UpdatePoctlPressed(q);
            }
        }
    }
}
