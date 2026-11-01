struct S { char pad[0x68]; void *f68; char pad2[0x78 - 0x6C]; int f78; };
extern char D_002DEB30[];
extern char D_002E65E0[];
void func_00164DF0(void *p, int n);
void func_001619A0(void *p, int n, struct S *ps);
void func_00162908(struct S *p);
void func_001ABD98(void *p, int n);
/** @sly2-annot addr=0x162F60
 * Purpose: Unless +0x78: set the end task complete (3); event 0x20; EndCurrentJob; unless +0x78 autosave
 *   (AutoSaveGame 0x1ABD98)
 * Info: proposed name FinishJob (T1); TU game
 * Confidence: mid (draft) [T1 0.50] - evidence: static analysis; sources: r3b_rest
 * See: docs/reference/functions/game-progression.md#game-0x1613a8-0x166270
 * @end-sly2-annot */
void func_00162F60(struct S *p)
{
    if (!p->f78)
        func_00164DF0(p->f68, 3);
    func_001619A0(D_002DEB30, 0x20, p);
    func_00162908(p);
    if (!p->f78)
        func_001ABD98(D_002E65E0, 0);
}
