extern struct { float t; float dt; } D_002DDED0;
struct C { virtual void f(); };
typedef void (C::*PMF)(void *);
struct D { char pad[0x60]; PMF pmf; };
struct OBJ {
    char pad0[0x14];
    float x14;
    float x18;
    char pad1c[0xF0 - 0x1C];
    D *xF0;
    C *xF4;
    void *xF8;
};
extern "C" void RemoveRip__FP3RIP(OBJ *);
/** @sly2-annot addr=0x1D0800
 * Purpose: SMACK Update (RemoveRip when done)
 * Info: proposed name UpdateSmack (T1); TU rip
 * Notes: names_v2 C proposed UpdateRip__FP3RIPf (0.200)
 * Confidence: high [T1 0.85] - evidence: static naming pass; sources: r5_1AE130+verdicts_tu_r3r5
 * See: docs/engine/particles-effects.md#6-key-functions,
 *   docs/reference/functions/particles-effects.md#rip-0x1cdcd0-0x1d1370
 * @end-sly2-annot */
extern "C" void UpdateSmack(OBJ *p)
{
    if (p->x18 < D_002DDED0.t - p->x14) {
        PMF pmf = p->xF0->pmf;
        (p->xF4->*pmf)(p->xF8);
        RemoveRip__FP3RIP(p);
    }
}
