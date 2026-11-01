// MATCH (Claude agent, laneL4, 2026-10-24): laneL3v6 (aligned 10) + a `GIFS *pgifs = &D_002DEA00;` local at the top of the j-loop body. Its loop-moved set-up now precedes the D_002BD3F0 one, so sched1 orders DEA00 before the high(D_002BD3F0) and the reload of &D_002BD398 lands in a0 like the target. No pins.
// laneL3v6 (Claude agent, 2026-10-24): aligned 10 (11 differing). laneL3v5 + the loop-1 counter i reused for t (i = state & 1): one global pseudo gives s4 for both like the target and lets sched2 put the andi in the call's delay slot. Left: j-preheader order only.
// laneL2v1 (Claude agent, 2026-10-24): aligned 82 (fresh1 90). C++ VU_VECTOR/VU_FLOAT class helpers in the j loop: pass-1 loop 128 / pass-2 101 insns, so 1.0/512/224 and HIGH(D_002BD410) stay in the loop and only f20 is saved. Left: extra dead VU temp stores at 0xC0, D_002BD3B0[j] still strength-reduced, &rw spilled instead of fp.
// fresh1 (Claude agent, 2026-10-23): aligned 90 (old 98): unsigned loop counters stop gcc reversing both loops (target uses sltiu). Left: p in s1 vs s3; the target's inner loop does not hoist 1.0/512/224/&D_002BD410 or strength-reduce D_002BD3B0[j]; ours hoists them all (f21-f23).
// DRAFT (Claude agent): HUD 2-matrix GIF sprite draw (state loop 0..2 over x164/x168/x16C; logic complete, regalloc/frame not yet: extra f21-f23, p in s1 vs s3).
typedef int qword __attribute__((mode(TI)));
typedef unsigned long ulong;
struct VECTOR4F { float x, y, z, w; };
struct M4 { qword r[4]; };
struct GIFS { int a[16]; };
struct TEX { ulong ul0; ulong pad; ulong ul10; };
struct X { char pad[0x24]; void *x24; char pad2[0x30 - 0x28]; int x30; };
struct P { char pad[0x60]; void *p60; char pad2[0x150 - 0x64]; int x150; int pad154; char *ap158[2]; int pad160; int x164; int x168; int x16C; };
extern "C" {
extern struct X *D_002DEA4C;
extern void *D_002DEA34;
extern struct GIFS D_002DEA00;
extern struct TEX D_002DECD0;
extern char D_002DF4B0[];
extern float D_002BD398[];
extern float D_002BD39C[];
extern struct VECTOR4F D_002BD3B0[4];
extern float D_002BD3F0[8];
extern struct M4 D_002BD410;
void func_001475C0(struct P *p);
void func_00147410(struct P *p, int n);
void func_001576F0(struct GIFS *pgifs);
void func_00157500(struct GIFS *pgifs, int n, void *pv);
void func_001574B8(struct GIFS *pgifs);
void func_00166BD8(struct GIFS *pgifs, int a, int b, int c);
void func_00166E90(struct GIFS *pgifs, int reg, ulong val);
void func_0022FF40(struct M4 *pm, int n, void *p, void *q);
qword func_0011C1A8(struct M4 *pm, qword v);
void Force__FPv(void *pv);
void func_00166D38(struct GIFS *pgifs, float s, float t, float q);
void func_00166C70(struct GIFS *pgifs, int rgba);
void func_00166DA0(struct GIFS *pgifs, int x, int y, int z);

}


union VECTOR4 { qword q; float f[4]; };
union VECTOR4;
struct VU_VECTOR { qword q; VU_VECTOR() {} VU_VECTOR(qword a) { q = a; } VU_VECTOR(const VECTOR4 &v); };
struct VU_VECTOR4 { qword q; VU_VECTOR4() {} VU_VECTOR4(qword a) { q = a; } VU_VECTOR4(const VECTOR4F &v) { q = *(qword *)&v; } };
struct VU_FLOAT { qword q; VU_FLOAT(float f) { __asm__("qmtc2.ni %1, %0" : "=j"(q) : "r"(f)); } };
inline VU_VECTOR::VU_VECTOR(const VECTOR4 &v) { q = v.q; }
VU_VECTOR4 operator*(const M4 &m, VU_VECTOR4 v);
static inline qword operator*(VU_FLOAT f, VU_VECTOR v)
{ qword r; __asm__("vmulx.xyzw %0, %2, %1x" : "=j"(r) : "j"(f.q), "j"(v.q)); return r; }
static inline void SetStq(GIFS *pgifs, const float &s, const float &t, const float &q) { func_00166D38(pgifs, s, t, q); }
static inline void SetRgba(GIFS *pgifs, const int &rgba) { func_00166C70(pgifs, rgba); }
static inline void SetXyz(GIFS *pgifs, const int &x, const int &y, const int &z) { func_00166DA0(pgifs, x, y, z); }
static inline int IFix(const float &g, const int &off) { return ((int)g + off) << 4; }
static inline float Screen(const float &g, const float &s) { return (g + 0.5f) * s + 0.5f; }

/** @sly2-annot addr=0x1477D8
 * Purpose: From main loop: draw the captured frame as two textured quads transformed by two child matrices
 *   (screen split/flip transition)
 * Info: proposed name DrawDancerScreen (T2); class DANCER; TU tu_1471C0
 * Notes: class identity from whole_game WE names/object strings: DrawCid68Screen -> DrawDancerScreen
 * Confidence: low (draft) [T2 0.40] - evidence: static analysis; sources: r2b_rest+whole_game(class identity)
 * See: docs/engine/cutscenes.md#3-screen-transitions, docs/engine/overview.md#2-the-frame-loop-exact-order,
 *   docs/reference/functions/cutscenes.md#tu_1471c0-0x1471c0-0x147e20
 * @end-sly2-annot */
extern "C" void func_001477D8(struct P *p)
{
    struct M4 am[2];
    float rw;
    unsigned int i;
    int state, stateNext;
    switch (p->x150) {
    case 1:
        func_001475C0(p);
        func_00147410(p, 2);
        break;
    case 2:
    case 3:
        break;
    default:
        return;
    }
    D_002DEA4C->x30 &= ~1;
    if (!D_002DEA4C->x24)
        D_002DEA4C->x24 = D_002DEA34;
    struct TEX *ptex = &D_002DECD0;
    func_001576F0(&D_002DEA00);
    func_00157500(&D_002DEA00, 0x25, D_002DF4B0);
    func_001574B8(&D_002DEA00);
    func_00166BD8(&D_002DEA00, 0, 1, 0xE);
    func_00166E90(&D_002DEA00, 0x47, 0x31001);
    func_00166E90(&D_002DEA00, 6, ((ptex->ul10 & 0x1FF) << 5) | (((ptex->ul0 >> 16) & 0x3F) << 14) | 0x6A8000000UL);
    func_00166E90(&D_002DEA00, 0x14, 0x61);
    func_00166E90(&D_002DEA00, 0x3F, 0);
    func_00166E90(&D_002DEA00, 8, 0x3800080000AUL);
    for (i = 0; i < 2; i++) {
        struct M4 m;
        func_0022FF40(&m, 0, p->p60, p->ap158[i] + 0x40);
        am[i] = m;
    }
    state = 0;
    do {
        switch (state) {
        case 0:
            stateNext = 1;
            if (!p->x164 || !p->x16C)
                goto next;
            break;
        case 1:
            stateNext = 2;
            if (!p->x168)
                goto next;
            break;
        case 2:
            stateNext = 3;
            if (!p->x164 || p->x16C)
                goto next;
            break;
        }
        {
            i = state & 1;
            unsigned int j;
            func_00166BD8(&D_002DEA00, 0x14, 3, 0x512);
            stateNext = state + 1;
            for (j = 0; j < 4; j++) {
                VECTOR4 u;
                GIFS *pgifs = &D_002DEA00; // one GIF packet pointer for the three per-vertex calls
                VU_VECTOR4 pos = am[i] * VU_VECTOR4(D_002BD3B0[j]);
                VU_VECTOR4 posClip = D_002BD410 * pos;
                u.q = posClip.q;
                Force__FPv(&u);
                rw = 1.0f / u.f[3];
                u.q = VU_FLOAT(rw) * VU_VECTOR(u);
                Force__FPv(&rw);
                Force__FPv(&u);
                u.f[0] = Screen(u.f[0], 512.0f);
                u.f[1] = Screen(u.f[1], 224.0f);
                Force__FPv(&u);
                SetStq(pgifs, rw * (D_002BD3F0[j * 2] + D_002BD398[i * 2]),
                       rw * (D_002BD3F0[j * 2 + 1] + D_002BD39C[i * 2]), rw);
                SetRgba(pgifs, 0x80808080);
                SetXyz(pgifs, IFix(u.f[0], 0x700), IFix(u.f[1], 0x790), 0);
            }
        }
    next:
        state = stateNext;
    } while (state < 3);
}
