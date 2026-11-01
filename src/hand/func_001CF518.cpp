typedef int qword __attribute__((mode(TI)));
struct M { qword a, b, c; };
struct S { char pad[0x40]; M m; qword q70; };
extern "C" {
int func_001CF338(S *p, qword q, M m);
void func_001CE980(S *p);
void func_001CF518(S *p);
}
/** @sly2-annot addr=0x1CF518
 * Purpose: RIP default Render (vt entry 6): FRenderRipPosMat, else RemoveRip
 * Info: proposed name RenderRip (T1); TU rip
 * Confidence: high [T1 0.85] - evidence: static analysis; sources: r5_1AE130+verdicts_A2+verify_side
 * See: docs/engine/particles-effects.md#2-rip-particles-ripcpp-0x1cdcd0-0x1d1370,
 *   docs/engine/particles-effects.md#6-key-functions, docs/engine/rendering.md#8-blips-hud-screen-effects
 * @end-sly2-annot */
void func_001CF518(S *p)
{
    if (!func_001CF338(p, p->q70, p->m)) func_001CE980(p);
}
