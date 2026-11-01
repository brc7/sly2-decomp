typedef int qword __attribute__((mode(TI)));
struct A { char pad[0x80]; float x80, y84, g88; char pad1[0x94 - 0x8C]; float g94; int n98; int n9C; };
struct B { char pad[0x1BC]; int x1BC; };
struct O { char pad[0x4C8]; int x4C8; int x4CC; char pad1[0x4D4 - 0x4D0]; struct B *p4D4; char pad2[0xDF0 - 0x4D8];
    char aDF0[0xF50 - 0xDF0]; char aF50[0x18C8 - 0xF50]; int n18C8; int x18CC; int n18D0; float g18D4;
    char pad3[0x1FF0 - 0x18D8]; char a1FF0[1]; };
struct CLOCK { float t; float dt; };
extern struct CLOCK D_002DDED0;
extern float D_002BF298;
extern float D_002BF29C;
extern void AdjustStepNewXp(struct O *p, struct A *pa, void *pv);
extern void func_0019E9C8(void *p, struct A *pa, void *pv);
extern void func_001A2100(void *p, struct A *pa, void *pv);
extern void func_001374E0(void *p, struct A *pa, void *pv);
static __inline__ float GClamp(float g, float lo, float hi)
{
    if (g < lo) return lo;
    if (g > hi) return hi;
    return g;
}
static __inline__ float GLimLo(float g, float lo, float v)
{
    if (g < lo) return v;
    return g;
}
/** @sly2-annot addr=0x188CC8
 * Purpose: JT vt55 (p, XP-like pa, pv): base STEP 0x204250, jts 8 -> 0x19E9C8, jts 7 friction-like scale g94
 *   = clamp(|v.xy|/g88+0.5, 0.5,1.5)
 * Info: proposed name AdjustJtXp (T2); class JT slot55; TU jt
 * Confidence: low (draft) [T2 0.30] - evidence: static analysis; sources: r4_17F090
 * See: docs/reference/functions/player.md#jt-0x188488-0x18e540
 * @end-sly2-annot */
void func_00188CC8(struct O *p, struct A *pa, void *pv)
{
    AdjustStepNewXp(p, pa, pv);
    switch (p->n18C8) {
    case 8:
        func_0019E9C8(p->aF50, pa, pv);
        break;
    case 7:
        if (pa->n9C == 0xE && p->x4C8 == 3 && p->p4D4->x1BC) {
            pa->n98 = 0;
            if (p->x4CC == 0) {
                pa->g94 = 0.0f;
            } else {
                qword l;
                float g;
                __asm__("vmul.xy %0, %1, %1\n\tvaddy.x %0, %0, %0y" : "=j"(l) : "j"(*(qword *)&pa->x80));
                __asm__("vsqrt Q, %1x\n\tvwaitq\n\tvaddq.x %0, $vf0, Q" : "=j"(l) : "j"(l));
                __asm__("qmfc2.ni %0, %1" : "=r"(g) : "j"(l));
                pa->g94 = GClamp(g / pa->g88 + 0.5f, 0.5f, 1.5f);
            }
        }
        break;
    case 4:
        func_001A2100(p->aDF0, pa, pv);
        break;
    case 6:
        func_001374E0(p->a1FF0, pa, pv);
        break;
    case 24:
        pa->g94 = 0.0f;
        pa->n98 = 0;
        break;
    case 5:
        if ((unsigned int)(p->n18D0 - 8) < 2) {
            register float g __asm__("$f0") = D_002DDED0.t - p->g18D4;
            if (g < D_002BF298) { g=D_002BF29C; pa->g94=g; }
        }
        break;
    }
}
