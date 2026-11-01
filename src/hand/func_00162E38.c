struct G { char pad[0x3B18]; int f3b18; char pad2[0x3C84 - 0x3B1C]; int f3c84; };
extern struct G D_003DD490;
int func_0020E040(struct G *p);
int func_00210B70(void);
void func_0022B490(struct G *p, int n, int a, int b, int c, int d);
/** @sly2-annot addr=0x162E38
 * Purpose: Records the job to abort (g_sw+0x3B18) and, unless a transition is busy, wipes back to the current
 *   world entry at default warp oid 0x189
 * Info: proposed name FailJob (T2); TU game
 * Notes: names_v2 v1 proposed FIgnoreTurretIntersection__FP6TURRETP2SO (0.000)
 * Confidence: low (draft) [T2 0.45] - evidence: static naming pass; sources: r3b_rest
 * See: docs/reference/functions/game-progression.md#game-0x1613a8-0x166270
 * @end-sly2-annot */
void func_00162E38(int n)
{
    struct G *pg = &D_003DD490;
    pg->f3b18 = n;
    if (!func_0020E040(pg) && !pg->f3c84)
        func_0022B490(pg, func_00210B70(), 0x189, -1, 0, -1);
}
