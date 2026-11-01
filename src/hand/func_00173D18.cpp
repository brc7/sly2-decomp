// func_00173D18 (Claude agent, 2026-10-25): MATCH. Updates the per-entry state of an array of 0x30-byte records (e->po object,
// e->k state, g4 max, g8 clock stamp). The VU helper layers (VsubXyzw, operator-, LengthXY, DistXYImpl, DistXY) pass VU_VECTORs
// by value; they add ~30 RTL insns to the loop (pass 1: 90, pass 2: 81 > threshold) so loop.c hoists the clock's %hi only in
// the second pass, giving the target's preheader order (1, hi BE5DC, giv 0, hi clock). Fewer layers (86) or more (94+) differ.
extern "C" {
typedef int qword __attribute__((mode(TI)));
struct CLOCK { float t; float dt; };
extern struct CLOCK D_002DDED0;
extern float D_002BE5DC;
struct O { char pad[0x28]; unsigned long fl; char pad2[0x84 - 0x30]; float g84; char pad3[0x330 - 0x88]; int n330; };
struct X { char pad[8]; struct O *po; };
struct E { struct O *po; float g4; float g8; struct X *px; int k; int pad[3]; qword pos; };
struct S { char pad[0x5C]; int c; struct E *ae; };
void func_00173928(struct S *p, struct E *pe, int k, int f);
}
struct VU_VECTOR { qword q; VU_VECTOR() {} VU_VECTOR(qword a) { q = a; } };
static inline VU_VECTOR VsubXyzw(VU_VECTOR a, VU_VECTOR b) { qword d; __asm__("vsub.xyzw %0, %1, %2" : "=j"(d) : "j"(a.q), "j"(b.q)); return VU_VECTOR(d); }
static inline VU_VECTOR SumXY(VU_VECTOR a) { qword d; __asm__("vmul.xy %0, %1, %1\n\tvaddy.x %0, %0, %0y" : "=j"(d) : "j"(a.q)); return VU_VECTOR(d); }
static inline VU_VECTOR Sqrt(VU_VECTOR a) { qword d; __asm__("vsqrt $Q, %1x\n\tvwaitq\n\tvaddq.x %0, $vf0, $Q" : "=j"(d) : "j"(a.q)); return VU_VECTOR(d); }
static inline float GetX(VU_VECTOR a) { float f; __asm__("qmfc2.ni %0, %1" : "=r"(f) : "j"(a.q)); return f; }
static inline VU_VECTOR operator-(VU_VECTOR a, VU_VECTOR b) { return VsubXyzw(a, b); }
static inline VU_VECTOR LengthXYV(VU_VECTOR a) { return Sqrt(SumXY(a)); }
static inline float LengthXY(VU_VECTOR a) { return GetX(LengthXYV(a)); }
static inline float DistXYImpl(VU_VECTOR a, VU_VECTOR b) { return LengthXY(a - b); }
static inline float DistXY(VU_VECTOR a, VU_VECTOR b) { return DistXYImpl(a, b); }
/** @sly2-annot addr=0x173D18
 * Purpose: Releases slots whose guard died or was disabled and re-arms slots once the player is far enough
 * Info: proposed name UpdateCid119Entries (T2); class CID119; TU tu_173158
 * Notes: Cid77->Cid119 (CID 0x77=119, agent-default(hex))
 * Confidence: low (draft) [T2 0.45] - evidence: static analysis; sources: r3b_rest
 * See: docs/reference/functions/guards-ai.md#tu_173158-0x173158-0x174898
 * @end-sly2-annot */
extern "C" void func_00173D18(struct S *p, qword pos) {
    int i;
    for (i = 0; i < p->c; i++) {
        struct E *e = &p->ae[i];
        struct O *o = e->po;
        if (!o) {
            if (e->k == 1) {
                int f = D_002BE5DC < DistXY(VU_VECTOR(e->pos), VU_VECTOR(pos));
                if (f) {
                    e->k = 0;
                    e->g8 = D_002DDED0.t;
                }
            }
        } else if (o->n330) {
            func_00173928(p, e, 1, 0);
        } else if (((int)(o->fl >> 60) & 1) == 0 || ((int)(e->px->po->fl >> 60) & 1) == 0) {
            func_00173928(p, e, 0, 0);
        } else {
            e->g4 = (e->g4 > o->g84) ? e->g4 : o->g84;
        }
    }
}
