struct VT { short d0; short p0; void *f0; char pad[0x40]; short d9; short p9; void (*pf9)(void *); };
struct S { char pad[0xC]; struct VT *pvt; char pad10[0x11C]; float x12C; float x130; char pad134[0x24]; int x158; char pad15c[8]; int x164; };
extern struct CLOCK { float t; float dt; } g_clock __asm__("D_002DDED0");
void UpdateLo(struct S *p);
/** @sly2-annot addr=0x21C0D8
 * Purpose: ramps uAlpha(0x12C) by duAlpha(0x130)*dt clamped 0..1; when faded out and idle>=count calls own vt
 *   slot9 (Remove)
 * Info: proposed name UpdateTrail (T1); class CID105 slot16; TU trail
 * Notes: agent qualifier: cid105 slot16
 * Confidence: mid (draft) [T1 0.55] - evidence: static analysis; sources: r8_21BB88+verdicts_tu_r7r8
 * See: docs/engine/particles-effects.md#6-key-functions,
 *   docs/reference/functions/particles-effects.md#trail-0x21bb88-0x21c820
 * @end-sly2-annot */
void func_0021C0D8(struct S *p)
{
    UpdateLo(p);
    if (p->x130 != 0.0f) {
        float t = p->x12C + g_clock.dt * p->x130;
        p->x12C = t;
        if (t <= 0.0f) {
            p->x12C = 0.0f;
            p->x130 = 0.0f;
        } else if (1.0f <= t) {
            p->x12C = 1.0f;
            p->x130 = 0.0f;
        }
    }
    if (p->x12C <= 0.0f && p->x130 <= 0.0f && !(p->x164 < p->x158))
        ((void (*)(void *))p->pvt->pf9)((char *)p + p->pvt->d9);
}
