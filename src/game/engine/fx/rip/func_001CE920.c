typedef int qword __attribute__((mode(TI)));
struct M3 { qword a, b, c; };
struct RIP { char pad[0x14]; float t14; float g18; int x1c; float g20; char pad2[0xC]; qword q30; struct M3 m; qword q70; qword q80; char pad3[0x50]; int ne0; };
extern float D_002DDED0;
extern qword D_002C1F50;
extern struct M3 D_002A2D00;
/** @sly2-annot addr=0x1CE920
 * Purpose: RIP default Init (vt entry 1): tCreated=g_clock.t, scale=1, pos/posPrev, identity matrix
 * Info: proposed name InitRip (T1); TU rip
 * Notes: verifier: purpose partly wrong: Sly 1 InitRip: psoTouch(+0xE0), tCreated=g_clock.t,
 *   dtLifetime(+0x18)=1.0, clqScale(+0x20)=scale arg, clqAlpha(+0x30)=g_clqAlphaRip, pos/posPrev, identity
 *   mat;
 * Confidence: high [T1 0.85] - evidence: Sly 1 / Lilly reference; sources: r5_1AE130+verdicts_tu_r3r5
 * See: docs/engine/particles-effects.md#6-key-functions,
 *   docs/reference/functions/particles-effects.md#rip-0x1cdcd0-0x1d1370
 * @end-sly2-annot */
void func_001CE920(struct RIP *p, qword pos, int n, float g)
{
    /*B*/p->ne0 = n;
    p->g18 = 1.0f;
    p->t14 = D_002DDED0;
    p->g20 = g;
    p->q30 = D_002C1F50;
    p->q80 = pos;
    p->q70 = pos;
    p->m = D_002A2D00;/*E*/
}
