typedef int qword __attribute__((mode(TI)));
struct M4 { qword a, b, c, d; };
struct V { float x, y, z, w; } __attribute__((aligned(16)));
struct VTE { short d; short pad; void (*pf)(); };
struct OBJ { VTE *vt; };
struct H { int x0; OBJ *pobj; };
struct Q { char pad[0x14]; void *x14; char p1[0x1C - 0x18]; float x1C; char p2[0x40 - 0x20]; float x40; };
struct R { char pad[0x30]; Q *x30; };
struct LO { char pad[0x12C]; float x12C; char p1[0x150 - 0x130]; int x150; char p2[0x168 - 0x154]; R *x168; };
extern "C" {
H *func_0012FF10(void *p, LO *plo, int a, int b);
qword func_001A62E8(LO *plo);
void *func_001A6318(LO *plo);
void RenderSoSelf(LO *plo);
void RenderDprizeSelf(LO *p);
}
M4 LoadMatrixFromPosRotScale(qword q, void *pv, V *pvec);
void func_001A5EE8(LO *plo, M4 m, float a, float b);
/** @sly2-annot addr=0x1C5FA0
 * Purpose: DPRIZE vt19 RenderSelf (bob/spin then SO render)
 * Info: proposed name RenderDprizeSelf (T1); class DPRIZE slot19; TU prize
 * Notes: verifier: purpose partly wrong: DPRIZE slot 19 (RenderSelf): skip in state 2; if the ASEGA at +0x168
 *   has channel 0xC, evaluate it and render with that matrix/scale (0x1A5EE8), else SO RenderSe
 * Confidence: high [T1 0.85] - evidence: static analysis; sources: r5_1AE130+verdicts_tu_r3r5
 * See: docs/engine/game-progression.md#8-key-functions,
 *   docs/reference/functions/game-progression.md#prize-0x1c4e50-0x1c8cc0
 * @end-sly2-annot */
void RenderDprizeSelf(LO *p)
{
    Q *q;
    H *ph;
    if (p->x150 == 2) return;
    if (p->x168 && (q = p->x168->x30) != 0) {
        ph = func_0012FF10(q->x14, p, 0xC, 1);
        if (ph) {
            V v;
            OBJ *pobj = ph->pobj;
            ((void (*)(void *, V *, int, float, float))pobj->vt[3].pf)((char *)pobj + pobj->vt[3].d, &v, 0, q->x1C, q->x40);
            M4 m;
            m = LoadMatrixFromPosRotScale(func_001A62E8(p), func_001A6318(p), &v);
            func_001A5EE8(p, m, p->x12C, p->x12C);
        } else {
            RenderSoSelf(p);
        }
    } else {
        RenderSoSelf(p);
    }
}
