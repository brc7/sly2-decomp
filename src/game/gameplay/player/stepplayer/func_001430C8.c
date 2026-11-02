typedef int qword __attribute__((mode(TI)));
struct P { char pad[0x10F8]; void *p10F8; int x10FC; };
extern float D_002BD2C0;
extern float D_002BD2C4;
qword func_001A62E8(void *p);
qword ConvertAloPos(void *pm, struct P *p, qword v);
float atan2f(float y, float x);
static __inline__ float GetX(qword q) { float x; __asm__("mtc1 %1,%0" : "=f"(x) : "r"(q)); return x; }
static __inline__ float GetY(qword q) { float y; __asm__("prot3w %0,%1" : "=r"(y) : "r"(q)); return y; }
static __inline__ float GAbs(float g) { return g < 0.0f ? -g : g; }
/** @sly2-annot addr=0x1430C8
 * Purpose: From lean target direction (atan2 in local space, quadrants) and nLeanKind return a bitmask (8,
 *   0x10<<n, 0x100<<n, 0x1000)
 * Info: proposed name GrfStepplayerLean (T2); TU stepplayer
 * Confidence: low (draft) [T2 0.45] - evidence: static analysis; sources: r2b_rest
 * See: docs/reference/functions/player.md#stepplayer-0x13ff70-0x144638,
 *   docs/reference/functions/player.md#jt-0x188488-0x18e540
 * @end-sly2-annot */
int func_001430C8(struct P *p)
{
    int n;
    int r;
    if (p->p10F8) {
        qword v = ConvertAloPos(0, p, func_001A62E8(p->p10F8));
        float g = atan2f(GetY(v), GetX(v));
        float a = __builtin_fabsf(g);
        if (a < D_002BD2C0) n = 0;
        else if (D_002BD2C4 < a) n = 3;
        else if (0.0f < g) n = 1;
        else n = 2;
    }
    r = 8;
    switch (p->x10FC) {
    case -1: case 0: break;
    case 1: r = 0x10 << n; break;
    case 2: r = 0x100 << n; break;
    case 3: r = 0x1000; break;
    case 4: r = 0x100; break;
    }
    return r;
}
