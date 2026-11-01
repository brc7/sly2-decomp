struct VTE { short d; short pad; void (*pf)(); };
struct LO { char pad[0xC]; struct VTE *vt; };
struct S { char pad[0x150]; int x150; struct LO *x154; struct LO *x158; };
int func_00132B10(struct LO *plo, int n);
struct LO *func_001CB250(struct LO *plo, int n);
void func_001C7A58(struct LO *plo, int id);
int func_001F4C58(struct S *p, void *pv, int n);
/** @sly2-annot addr=0x1C2DC8
 * Purpose: Attach loot: from a PROXY (class 6, PloAcquireProxy) or an SO; clue (0x5E) gets id, prize (0x5A)
 *   state 2 else vt9; find attach XF; returns status 0-3
 * Info: proposed name StageCid99Loot (T2); class CID99; TU tu_1C2D20
 * Confidence: low (draft) [T2 0.45] - evidence: static analysis; sources: r5b_rest
 * See: docs/reference/functions/game-progression.md#prize-0x1c4e50-0x1c8cc0,
 *   docs/reference/functions/gameplay-objects.md#tu_1c2d20-0x1c2d20-0x1c3390,
 *   docs/reference/functions/guards-ai.md#pickpocket-0x1f1768-0x1f2108
 * @end-sly2-annot */
int func_001C2DC8(struct S *p, struct LO *plo, int id, void *pv)
{
    if (p->x158) return 0;
    p->x154 = 0;
    if (func_00132B10(plo, 6)) {
        p->x154 = plo;
        p->x158 = func_001CB250(plo, 0);
        if (!p->x158) {
            p->x154 = 0;
            return 1;
        }
    } else if (func_00132B10(plo, 0x30)) {
        p->x158 = plo;
    } else {
        return 2;
    }
    if (func_00132B10(p->x158, 0x5E)) func_001C7A58(p->x158, id);
    if (func_00132B10(p->x158, 0x5A))
        ((void (*)(void *, int))p->x158->vt[76].pf)((char *)p->x158 + p->x158->vt[76].d, 2);
    else
        ((void (*)(void *))p->x158->vt[9].pf)((char *)p->x158 + p->x158->vt[9].d);
    p->x150 = func_001F4C58(p, pv, 0xD);
    return 3;
}
