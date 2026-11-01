struct VTE { short d; short pad; void (*pfn)(void *); };
struct O { char pad[0xC]; struct VTE *vt; };
struct S { char pad[0x3BC4]; struct O *po; char pad2[0x476C - 0x3BC8]; int n; };
/** @sly2-annot addr=0x161958
 * Purpose: Hides HUD widget g_sw+0x3BC4 (vt50 = WIDGET hide 0x2129E8) and clears g_sw+0x476C
 * Info: proposed name HideSwWidget3BC4 (T2); TU game
 * Notes: names_v2 C proposed CosRotateMatrixMagnitude__FP7MATRIX3 (0.638)
 * Confidence: low (draft) [T2 0.40] - evidence: static naming pass; sources: r3b_rest
 * See: docs/reference/functions/game-progression.md#game-0x1613a8-0x166270
 * @end-sly2-annot */
void func_00161958(struct S *p)
{
    struct O *po = p->po;
    if (po) {
        struct VTE *e = &po->vt[50];
        e->pfn((char *)po + e->d);
    }
    p->n = 0;
}
