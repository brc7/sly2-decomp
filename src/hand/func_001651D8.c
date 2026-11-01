struct G;
struct Q { char pad[0x10]; int f10; int f14; int f18; };
extern struct G D_003DD490;
extern struct Q D_002DEB30;
void *func_0015E938(struct G *pg, int kind, int oid, void *owner);
void func_00165E10(struct G *pg, void *p);
void func_00165538(void *p);
void func_0022B490(struct G *pg, int n, int a, int b, int c, int d);
/** @sly2-annot addr=0x1651D8
 * Purpose: Finds the TASK by oid and GoToTask, else stores the oid as pending in g_game and warps to the
 *   given level
 * Info: proposed name StartTaskByOid (T2); TU game
 * Confidence: low (draft) [T2 0.45] - evidence: static analysis; sources: r3b_rest
 * See: docs/reference/functions/game-progression.md#game-0x1613a8-0x166270
 * @end-sly2-annot */
void func_001651D8(void *pv, int n, int oid)
{
    struct G *pg = &D_003DD490;
    void *p = func_0015E938(pg, 0x105, oid, 0);
    if (p) {
        func_00165E10(pg, p);
        func_00165538(p);
    } else {
        D_002DEB30.f10 = -1;
        D_002DEB30.f14 = -1;
        D_002DEB30.f18 = oid;
        func_0022B490(pg, n, 0x189, -1, 8, -1);
    }
}
