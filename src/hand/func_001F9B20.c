typedef int qword __attribute__((mode(TI)));
struct E { char pad[0x80]; qword v80; char pad2[0x20]; char subB0[0x10]; int nC0; };
struct P { char pad[0x58]; int n58; char pad2[0xE8 - 0x5C]; struct E *pe; char pad3[0x138 - 0xEC]; int n138; };
extern qword D_002A2DD0;
int func_0011D4D0(float, float, float);
qword func_001FA810(void *p, int n, qword v);
void func_001FAC30(struct P *p, qword v);
void func_00229798(struct P *p);
static inline float X(qword q) { float f; __asm__("mtc1 %1, %0" : "=f"(f) : "r"(q)); return f; }
static inline float Y(qword q) { float f; __asm__("prot3w %0, %1" : "=r"(f) : "r"(q)); return f; }
static inline float Z(qword q) { float f; __asm__("pextuw %0, $0, %1" : "=r"(f) : "r"(q)); return f; }
static inline int FNear(qword a, qword b)
{
    return func_0011D4D0(X(a), X(b), 0.0001f) && func_0011D4D0(Y(a), Y(b), 0.0001f) && func_0011D4D0(Z(a), Z(b), 0.0001f);
}
/** @sly2-annot addr=0x1F9B20
 * Purpose: If SO has nonzero pending velocity (pe+0x80) and not locked (kind!=3), constrain it (0x1FA810) and
 *   add via 0x1FAC30; then 0x229798 if +0x138
 * Info: proposed name ApplySoPendingVelocity (T2); class SO slot70; TU solve
 * Notes: names_v2 C proposed RebuildShadowRegion__FP6SHADOW (0.438)
 * Confidence: low (draft) [T2 0.30] - evidence: static naming pass; sources: r7_1F8DD8
 * See: docs/reference/functions/guards-ai.md#step-0x203948-0x205e70,
 *   docs/reference/functions/physics-collision.md#solve-0x1f8dd8-0x1faf70,
 *   docs/reference/functions/player.md#jt-0x188488-0x18e540
 * @end-sly2-annot */
void func_001F9B20(struct P *p)
{
    int fUpd = 0;
    int fNear = FNear(D_002A2DD0, p->pe->v80);
    if (!fNear) fUpd = p->pe->nC0 != 3;
    if (fUpd) func_001FAC30(p, func_001FA810(p->pe->subB0, p->n58, p->pe->v80));
    if (p->n138) func_00229798(p);
}
